#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include "BBQ.h"
#define N 2 //10 treads "dont tread on me"

using namespace std;

BBQ *qq = new BBQ(10);

void *add_function(void *q)
{
   int i = 0;

    while (1)
    {
        qq->insert(i);
        cout <<"ADDING SHIT\n";
        i++;
        sleep(1);
    }
}

void *remove_function(void *q)
{
    int id;
    int i = 0;

    while (1)
    {
        id = qq->remove();
        printf("REMOVING %d : (iter) %d (Queue Size) %d\n", id, i, qq->size());
        i++;
        sleep(10);
    }
}

int main(int, char **)
{
    // Create producer and consumer threads

    pthread_t p_thread[N];
    int thr_add, thr_rem;
    int status;
    int a = 1;
    int b = 2;

for(int i=0;i<N/2+1;i++) {
    thr_add = pthread_create(&p_thread[i], NULL, add_function, &a);
    if (thr_add < 0)
    {
        perror("thread create error on add_function: ");
        cout << i << endl;
        exit(0);
    }
}

for(int i=N/2;i<N;i++) {
    thr_rem = pthread_create(&p_thread[i], NULL, remove_function, &b);
    if (thr_rem < 0)
    {
        perror("thread create error on remove_function: ");
        cout << i << endl;
        exit(0);
    }
}

// Join threads plz
for(int i=0;i<N;i++)
    pthread_join(p_thread[i], NULL);

    return 0;
}
