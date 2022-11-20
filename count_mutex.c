/******************************************************************************
 * compile with gcc -pthread *.c -o loops
 * test with valgrind --tool=helgrind ./lops
 *
 ******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4
#define ARRAYSIZE 16
#define ITERATIONS ARRAYSIZE / NTHREADS

int count = 0;
int a[ARRAYSIZE];
pthread_mutex_t count_mutex;

void *do_work(void *tid)
{
    int i, start, *mytid, end;
    int myCount = 0;

    mytid = (int *)tid;
    start = (*mytid * ITERATIONS);
    end = start + ITERATIONS;
    printf("\n[Thread %5d] Doing iterations \t%10d to \t %10d", *mytid, start, end - 1);
    for (i = start; i < end; i++)
    {
        if (a[i] == 1)
        {
            pthread_mutex_lock(&count_mutex);
            count++;
            pthread_mutex_unlock(&count_mutex);

        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    for (int i = 0; i < ARRAYSIZE; i++)
    {
        a[i] = 1;
    }

    int i, start, tids[NTHREADS];
    pthread_t threads[NTHREADS];
    pthread_attr_t attr;

    pthread_mutex_init(&count_mutex, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i = 0; i < NTHREADS; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], &attr, do_work, (void *)&tids[i]);
    }

    /* Wait for all threads to complete then print global sum */
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("\n[MAIN] Done. count= %d", count);

    count = 0;
    /* for (i=0;i<ARRAYSIZE;i++){
     a[i] = i*1.0;
     sum = sum + a[i]; }
     printf("\n[MAIN] Check Sum= %e",sum);
   */
    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_exit(NULL);
}