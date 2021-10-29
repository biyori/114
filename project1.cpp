#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include "BBQ.h"

using namespace std;

static BBQ *qq = new BBQ(10);

void *add_function(void *q)
{
    int id;
    int i = 0;
    id = 12;

    while (1)
    {
        qq->insert(id);
        i++;
        sleep(1);
    }
}

void *remove_function(void *q)
{
    int id;
    int i = 0;
    id = 12;

    while (1)
    {
        qq->remove();
        printf("%d : %d\n", id, i);
        i++;
        sleep(1);
    }
}

int main(int, char **)
{
    // Create producer and consumer threads

    pthread_t p_thread[2];
    int thr_id0,thr_id1;
    int status;
    int a = 1;
    int b = 2;

    thr_id0 = pthread_create(&p_thread[0], NULL, add_function, &a);
    if (thr_id0 < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    thr_id1 = pthread_create(&p_thread[1], NULL, remove_function, &b);
    if (thr_id1 < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1],NULL);

    cout << qq->size() << endl;

    return 0;
}
