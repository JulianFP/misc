#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

pthread_mutex_t mutex;

int numThreads;
int* count;
int contest_running = 1;

void* contest(void *argument) {
    while(contest_running == 1){
        int tid;
        tid = *((int *) argument);
        pthread_mutex_lock(&mutex);
        ++count[tid];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    //get number of threads from cli argument
    if(argc < 2){
        printf("The number of threads is a required argument\n");
        return EXIT_FAILURE;
    }
    numThreads = atoi(argv[1]);
    
    //initialize global variables
    int countArray[numThreads];
    count = countArray; //count pointer points to same array
    pthread_t thread[numThreads];
    pthread_mutex_init(&mutex, NULL);

    int rc;
    int thread_args[numThreads];
    for(int i = 0; i < numThreads; ++i){
        thread_args[i] = i;
        rc = pthread_create(&thread[i], NULL, &contest, (void *) &thread_args[i]);
        assert(rc == 0);
    }

    //ensure that all threads are running
    sleep(1);

    //reset counters
    for(int i = 0; i < numThreads; ++i){
        count[i] = 0;
    }

    //let it run
    sleep(3);

    //stop contest, wait for threads to complete and return counts
    contest_running = 0;
    int sum = 0;
    for(int i = 0; i < numThreads; ++i){
        rc = pthread_join(thread[i], NULL);
        assert(rc == 0);

        printf("Thread number %d acquired the mutex ", i);
        printf("%d times\n", count[i]);
        sum += count[i];
    }
    printf("In total the mutex was acquired %d times\n", sum);

    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}
