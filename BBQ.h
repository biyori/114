#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

class BBQ
{
    struct statistics_t
    {
        int addedHits;
        int addedWaitHits;
        int removedHits;
        int removedWaitHits;
    };

    struct bbq_item
    {
        string name;
        int id;
    };

private:
    // Synchronization variables
    mutex the_lock;
    condition_variable itemAdded;
    condition_variable itemRemoved;
    // Use vector for dynamic allocation--set max size in constructor
    vector<bbq_item> items; //push back & pop back mimic queue behavior
    int front, nextEmpty;
    int max_size;
    int addedProbe, removeWaitProbe, removedProbe, addedWaitProbe;

public:
    BBQ(int);
    ~BBQ(){};
    void insert(int, string, int);
    bbq_item remove(int);
    int size();
    statistics_t get_stats();
    void printStats();
};

BBQ::BBQ(int size)
{
    max_size = size;
    front = nextEmpty = 0;
    addedProbe = removeWaitProbe = removedProbe = addedWaitProbe = 0;
}
void BBQ::insert(int item, string name, int threadID)
{
    unique_lock<mutex> lock(the_lock);
    while ((nextEmpty - front) == max_size)
    {
        cout << "Waiting to produce by thread number #" << threadID << endl;
        addedWaitProbe++; // Total wait hits for adding items
        itemRemoved.wait(lock);
    }
    addedProbe++; // Total items added
    cout << "Item ID #" << item << " (" << name << ") produced by thread number #" << threadID << endl;
    items.push_back(bbq_item{name, item});
    nextEmpty++;
    itemAdded.notify_all();
    lock.unlock();
}
BBQ::bbq_item BBQ::remove(int threadID)
{
    bbq_item tmp;
    unique_lock<mutex> lock(the_lock);
    while ((nextEmpty - front) == 0)
    {
        cout << "Waiting to consume by thread number #" << threadID << endl;
        removeWaitProbe++; // Total wait hits for removing items
        itemAdded.wait(lock);
    }
    removedProbe++; // Total items removed
    tmp = items.back();
    cout << "Item ID #" << tmp.id << " (" << tmp.name << ") consumed by thread number #" << threadID << endl;
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

BBQ::statistics_t BBQ::get_stats()
{
    return statistics_t{addedProbe, removeWaitProbe, removedProbe, addedWaitProbe};
}
void BBQ::printStats()
{
    printf("Added: %2d Add Wait: %2d Removed: %2d Remove Wait: %2d\n", addedProbe, addedWaitProbe, removedProbe, removeWaitProbe);
}