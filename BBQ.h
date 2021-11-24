#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

class BBQ
{
private:
    // Synchronization variables
    mutex the_lock;
    condition_variable itemAdded;
    condition_variable itemRemoved;
    // Use vector for dynamic allocation--set max size in constructor
    vector<int> items;
    int front, nextEmpty;
    int max_size;

public:
    BBQ(int);
    ~BBQ(){};
    void insert(int item, int id);
    int remove(int id);
    int size();
};

BBQ::BBQ(int size)
{
    max_size = size;
    front = nextEmpty = 0;
}
void BBQ::insert(int item, int threadID)
{
    unique_lock<mutex> lock(the_lock);
    while ((nextEmpty - front) == max_size)
    {
        cout << "Waiting to produce by thread number #" << threadID << endl;
        itemRemoved.wait(lock);
    }
    cout << "Item ID #" << item << " produced by thread number #" << threadID << endl;
    items.push_back(item);
    nextEmpty++;
    itemAdded.notify_all();
    lock.unlock();
}
int BBQ::remove(int threadID)
{
    int tmp;
    unique_lock<mutex> lock(the_lock);
    while ((nextEmpty - front) == 0)
    {
        cout << "Waiting to consume by thread number #" << threadID << endl;
        itemAdded.wait(lock);
    }
    tmp = items.back();
    cout << "Item ID #" << tmp << " consumed by thread number #" << threadID << endl;
    items.pop_back();
    nextEmpty--;
    itemRemoved.notify_all();
    lock.unlock();
    return tmp;
}
int BBQ::size()
{
    return items.size();
}