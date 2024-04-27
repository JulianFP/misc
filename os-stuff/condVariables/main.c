#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mut;
pthread_cond_t cond;

int progress = 0;

void thread1(){
    pthread_mutex_lock(&mut);
    printf("A\n");
    ++progress;
    pthread_cond_signal(&cond);
    while(progress < 2){
        pthread_cond_wait(&cond, &mut);
    }
    printf("C\n");
    ++progress;
}

void thread2(){
    pthread_mutex_lock(&mut);
    while(progress < 1){
        pthread_cond_wait(&cond, &mut);
    }
    printf("B\n");
    ++progress;
    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);
}


int main(int argc, char *argv[])
{
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
