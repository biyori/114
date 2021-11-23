#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "BBQ.h"
#define THREADS 20
#define BBQ_CAPACITY 10

using namespace std;

BBQ *qq = new BBQ(BBQ_CAPACITY);
int bonk(int, int);

void *add_bonk(void *slep) // TP
{
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    //pid_t tid = gettid();
    int id = tid;
    int i = 0;
    int slp = *((int *)slep);

    while (1)
    {
        qq->insert(i, id);
        cout << "ADDING SHIT\n";
        i++;
        cout << "SLEP FOR: " << bonk(qq->size(), slp) << endl;
        sleep(bonk(qq->size(), slp)); //queue_Size, current_q_size, sleep
    }
}

void *remove_bonk(void *slep) // TC
{
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    //pid_t tid = gettid();
    int id = tid;
    int i = 0;
    int slp = *((int *)slep);
    cout << "WE SLEP ->" << slp << endl;
    while (1)
    {
        id = qq->remove(id);
        //printf("REMOVING %d : (iter) %d (Queue Size) %d\n", id, i, qq->size());
        i++;
        cout << "SLEP FOR: " << bonk(qq->size(), slp) << endl;
        sleep(bonk(qq->size(), slp)); //queue_Size, current_q_size, sleep
    }
}

int bonk(int currentQueueCapacity, int currentSleep)
{
    // int sleepModifier = 1;
    //     if (BBQ->getQueueCapacity() > slowdownThreshold)
    //         sleepModifier = 1.0 + (BBQ->getQueueCapacity() - slowdownThreshold) / (1.0 - slowdownThreshold);
    //     else if (BBQ->getQueueCapacity() < (1 - slowdownThreshold))
    //         sleepModifier = BBQ->getQueueCapacity() / (1 - slowdownThreshold);
    //     int sleepTime = (rand() % sleepRange) * sleepModifier;
    //     this_thread::sleep_for(chrono::milliseconds(sleepTime));

    bool slowBonk = currentQueueCapacity >= BBQ_CAPACITY - (BBQ_CAPACITY * 0.25);
    int TIDDY = currentSleep * 2; // THREAD ID SLEEP
    if (slowBonk)
        return currentSleep + TIDDY;
    //1.0 + (BBQ->getQueueCapacity() - slowdownThreshold) / (1.0 - slowdownThreshold);
    assert(TIDDY / 6 * currentQueueCapacity < currentSleep);
    return currentSleep - (TIDDY / 6 * currentQueueCapacity);
}

int main(int argc, char **argv)
{
    // Need more arguments [TC] and [TP]
    if (argc != 3)
    {
        cout << "Error missing arguments: project1 TC TP" << endl;
        return -2;
    }

    cout << argv[1];
    cout << argv[2];

    // Create producer and consumer threads
    pthread_t p_thread[THREADS];
    int thr_add, thr_rem;
    int status;
    int TP = atoi(argv[2]);
    int TC = atoi(argv[1]);

    for (int i = 0; i < THREADS / 2 + 1; i++)
    {
        thr_add = pthread_create(&p_thread[i], NULL, add_bonk, &TP);
        if (thr_add < 0)
        {
            perror("thread create error on add_function: ");
            cout << i << endl;
            exit(0);
        }
    }

    for (int i = THREADS / 2; i < THREADS; i++)
    {
        thr_rem = pthread_create(&p_thread[i], NULL, remove_bonk, &TC);
        if (thr_rem < 0)
        {
            perror("thread create error on remove_function: ");
            cout << i << endl;
            exit(0);
        }
    }

    // Join threads plz
    for (int i = 0; i < THREADS; i++)
        pthread_join(p_thread[i], NULL);

    return 0;
}
