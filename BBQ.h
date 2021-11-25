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
    vector<bbq_item> items; // push back & pop back mimic queue behavior
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
        cout << "\033[33mWaiting to produce by thread number #" << threadID << "\033[0m" << endl;
        addedWaitProbe++; // Total wait hits for adding items
        itemRemoved.wait(lock);
    }
    addedProbe++; // Total items added
    cout << "\033[32mItem ID #" << item << "\t(" << name << ")   \tproduced by thread number #" << threadID << "\033[0m" << endl;
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
        cout << "\033[33mWaiting to consume by thread number #" << threadID << "\033[0m" << endl;
        removeWaitProbe++; // Total wait hits for removing items
        itemAdded.wait(lock);
    }
    removedProbe++; // Total items removed
    tmp = items.back();
    cout << "\033[31mItem ID #" << tmp.id << "\t(" << tmp.name << ")   \tconsumed by thread number #" << threadID << "\033[0m" << endl;
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
    printf("\033[1;37mProducer\n| \033[0;32mProduced: %2d\n| \033[33mWait: %6d\n\033[1;37mConsumer\n| \033[0;31mConsumed: %2d\n| \033[33mWait: %6d\033[0m\n", addedProbe, addedWaitProbe, removedProbe, removeWaitProbe);
}