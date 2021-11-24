#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "BBQ.h"
#define THREADS 20
#define BBQ_CAPACITY 10

using namespace std;

BBQ *qq = new BBQ(BBQ_CAPACITY);
int bonk(int, int);

struct pthread_args
{
    int sleep;
    int threadId;
};

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

    while (1)
    {
        qq->insert(i, threadName);
        i = qq->get_stats().addedHits;
        sleep(bonk(qq->size(), slp));
    }
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
    int i = 0;
    pthread_args args = *((pthread_args *)TC);
    int slp = args.sleep;
    int threadName = args.threadId;

    while (1)
    {
        qq->remove(threadName);
        i++;
        sleep(bonk(qq->size(), slp));
        qq->printStats();
    }
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    cerr << "Error this program will only run on Mac, Unix, or Linux";
    return 0;
#endif

    // Need more arguments [TC] and [TP]
    if (argc != 3)
    {
        for (int i = 0; i < BBQ_CAPACITY; i++)
            printf("Queue Size [%d] sleeps for => %2d\n", i, bonk(i, 10));

        cout << "Producers: ";
        for (int i = 0; i < (THREADS / 2); i++)
            cout << i << " ";
        cout << "\nConsumers: ";
        for (int i = THREADS / 2; i < THREADS; i++)
            cout << i << " ";
        cout << endl;

        cerr << "Error missing arguments: project1 TC TP" << endl;
        return -2;
    }

    // Parse the argument chars to integers
    int TP = atoi(argv[2]);
    int TC = atoi(argv[1]);

    // Create producer and consumer threads
    pthread_t p_thread[THREADS];
    int thr_add, thr_rem;
    pthread_args _args;
    for (int i = 0; i < THREADS / 2; i++)
    {
        _args = {bonk(i, TP), i};
        thr_add = pthread_create(&p_thread[i], NULL, add_bonk, &_args);
        if (thr_add < 0)
        {
            perror("thread create error on add_function: ");
            cerr << i << endl;
            exit(0);
        }
        else
        {
            //Thread creation
            cout << "Creating producer thread #" << i << endl;
        }
    }

    for (int i = THREADS / 2; i <= THREADS; i++)
    {
        _args = {bonk(i, TC), i};
        thr_rem = pthread_create(&p_thread[i], NULL, remove_bonk, &_args);
        if (thr_rem < 0)
        {
            perror("thread create error on remove_function: ");
            cerr << i << endl;
            exit(0);
        }
        else
        {
            cout << "Creating consumer thread #" << i << endl;
        }
    }

    // Join threads
    for (int i = 0; i < THREADS; i++)
        pthread_join(p_thread[i], NULL);

    return 0;
}

int bonk(int currentQueueCapacity, int currentSleep)
{
    srand(time(NULL));
    //below 25% occupancy and reaches twice the initial average speed when buffer is empty
    // If the current queue capacity is greater than or equal to 75% full
    // Slow mode
    bool slowBonk = currentQueueCapacity >= BBQ_CAPACITY * 0.75;      // 75% full
    bool oneFourthBonk = currentQueueCapacity <= BBQ_CAPACITY * 0.25; // 0-25% filled
    if (slowBonk)
        return currentSleep * (currentQueueCapacity / (double)BBQ_CAPACITY) + 1; // Sleep for a fraction of the size of the currently filled queue -- up tp 100%
    else if (oneFourthBonk)
        return rand() % (currentQueueCapacity + 1);
    else
        return rand() % (int)(currentQueueCapacity - (currentQueueCapacity * 0.25) + 2); // From 25-75
}