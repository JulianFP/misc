#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t AbeforeB;
sem_t BbeforeC;

void thread1(){
    printf("A\n");
    sem_post(&AbeforeB);
    sem_wait(&BbeforeC);
    printf("C\n");
}

void thread2(){
    sem_wait(&AbeforeB);
    printf("B\n");
    sem_post(&BbeforeC);
}

int main(int argc, char *argv[])
{
    sem_init(&AbeforeB, 0, 0);
    sem_init(&BbeforeC, 0, 0);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&AbeforeB);
    sem_destroy(&BbeforeC);
    return 0;
}
