#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <chrono>
#include "BBQ.h"
#define THREADS 20
#define BBQ_CAPACITY 50 // Buffer sizes (25, 50, 100)

using namespace std;

BBQ *grill = new BBQ(BBQ_CAPACITY);
int bonk(int, int);
const string bbq_items[] = {"Hotdogs", "Hamburger", "Corn", "Shrimp", "Buns", "BBQ Sauce", "Kebabs", "Onions", "Peppers", "Ribs", "Chicken", "Sausage", "Brisquet"};
chrono::steady_clock::time_point startTime;
string randomGrillItem(); // returns a random bbq_item
void stats_handler(int);
pthread_t *instance;

struct pthread_args
{
    int sleep;
    int threadId;
};

void cleanup_handler(void *arg)
{
    printf("Called clean-up handler\n");
}

void *add_bonk(void *TP) // Thread Producer
{
#ifdef __APPLE__
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
#endif
#ifdef __CYGWIN__
    int tid = static_cast<int>(pthread_self());
#endif
#ifdef __linux__
    pid_t tid = gettid();
#endif
    int id = tid;
    int i = 0;
    pthread_args args = *((pthread_args *)TP);

    int slp = args.sleep; //*((int *)TP_Sleep);
    int threadName = args.threadId;

    //  pthread_cleanup_push(cleanup_handler, NULL);

    while (1)
    {
        grill->insert(i, randomGrillItem(), threadName);
        i = grill->get_stats().addedHits;
        sleep(bonk(grill->size(), slp));
        pthread_testcancel(); // Check if thread should be terminated -- signal handler will send the termination request
    }
    // pthread_cleanup_pop(threadName);
    return NULL;
}

void *remove_bonk(void *TC) // Thread Consumer
{
#ifdef __APPLE__
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
#endif
#ifdef __CYGWIN__
    int tid = static_cast<int>(pthread_self());
#endif
#ifdef __linux__
    pid_t tid = gettid();
#endif
    int id = tid;
    pthread_args args = *((pthread_args *)TC);
    int slp = args.sleep;
    int threadName = args.threadId;

    //  pthread_cleanup_push(cleanup_handler, NULL);

    while (1)
    {
        grill->remove(threadName);
        sleep(rand() % slp);
        pthread_testcancel(); // Check if thread should be terminated -- signal handler will send the termination request
    }
    //  pthread_cleanup_pop(threadName);
    return NULL;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    cerr << "Error this program will only run on Mac, Unix, or Linux";
    return 0;
#endif
    srand(time(NULL));
    if (argc != 3)
    {
        for (int i = 0; i < BBQ_CAPACITY; i++)
            printf("Queue Size [%d] sleeps for => %2d\n", i, bonk(i, 4));

        cout << "Producers Threads: ";
        for (int i = 0; i < THREADS / 2; i++)
            printf("%3d", i);
        cout << "\nConsumers Threads: ";
        for (int i = THREADS / 2; i < THREADS; i++)
            printf("%3d", i);
        cout << endl;

        cerr << "\033[31mError missing arguments: \033[0m";
        cerr << "\033[33m./project1 TC TP\033[0m" << endl;
        return -2;
    }

    // Parse the argument chars to integers
    int TC = atoi(argv[1]); // Consumer
    int TP = atoi(argv[2]); // Producer

    if (TC < 2 || TP < 2)
    {
        cerr << "\033[31mError TC & TP must be greater than 1\033[0m" << endl;
        return -3;
    }

    // Create producer and consumer threads
    pthread_t p_thread[THREADS];
    int thr_add, thr_rem;
    pthread_args _args;
    for (int i = 0; i < THREADS / 2; i++)
    {
        _args = {TP, i};
        thr_add = pthread_create(&p_thread[i], NULL, add_bonk, &_args);
        if (thr_add < 0)
        {
            perror("thread create error on add_function: ");
            cerr << i << endl;
            exit(0);
        }
        else
        {
            // Thread creation
            cout << "\033[1;35mCreating producer thread #" << i << "\033[0m" << endl;
        }
    }

    for (int i = THREADS / 2; i <= THREADS; i++)
    {
        _args = {TC, i};
        thr_rem = pthread_create(&p_thread[i], NULL, remove_bonk, &_args);
        if (thr_rem < 0)
        {
            perror("thread create error on remove_function: ");
            cerr << i << endl;
            exit(0);
        }
        else
        {
            cout << "\033[1;35mCreating consumer thread #" << i << "\033[0m" << endl;
        }
    }
    // Start Timer
    startTime = chrono::steady_clock::now();
    signal(SIGINT, stats_handler);

    instance = p_thread;
    // Join/run threads
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(p_thread[i], NULL);
    }

    delete grill;
    //delete instance; -- not needed since instance is a pointer but it was not allocated with "new"

    cout << "Quit successfully" << endl;

    return 0;
}

int bonk(int currentQueueCapacity, int currentSleep)
{
    // srand(time(NULL));
    // below 25% occupancy and reaches twice the initial average speed when buffer is empty
    //  If the current queue capacity is greater than or equal to 75% full
    //  Slow mode
    bool slowBonk = currentQueueCapacity >= BBQ_CAPACITY * 0.75;      // 75% full
    bool oneFourthBonk = currentQueueCapacity <= BBQ_CAPACITY * 0.25; // 0-25% filled
    if (slowBonk)
    {
        // cout << "75% ";
        return min((rand() % (currentSleep - 1 * (currentQueueCapacity / BBQ_CAPACITY)) + currentSleep), currentSleep); // Sleep for a fraction of the size of the currently filled queue -- up tp 100%
    }
    else if (oneFourthBonk)
    {
        // cout << "1/4th ";
        return min((rand() % (currentQueueCapacity + 1)), (int)(currentSleep * 0.25));
    }
    else
    {
        // cout << "25-75 ";
        return min((rand() % (int)(currentQueueCapacity * 0.25) + (int)(currentQueueCapacity * 0.75)), (int)(currentSleep * 0.75)); // Use min in case the currentQueue exceeds the currentSleep time
    }
}

void stats_handler(int s)
{
    auto stopTime = chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime).count();
    cout << endl;
    grill->printStats();

    cout << "\033[1;32m"
         << "Ran for: " << duration << " seconds"
         << "\033[0m" << endl;

    // Send cancellation request
    for (int i = 0; i < THREADS; i++)
    {
        pthread_cancel(instance[i]);
    }
}

string randomGrillItem()
{
    return bbq_items[rand() % bbq_items->size()];
}