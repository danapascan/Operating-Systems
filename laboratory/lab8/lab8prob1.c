#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <semaphore.h>
 
 long count=0;
 sem_t semafor;
 void *thread_function(void *unused)
 {
    int i;
    long aux;
    int M=1000;
    for(i=0; i<M; i++){
    sem_wait( &semafor);
    aux = count;
    aux++;
    usleep(random() % 10);
    count = aux;
     sem_post(&semafor);
    }
    return NULL;
 }

 int main()
 {
    if(sem_init(&semafor,0,1)!=0)
    {
        perror("Could not init the semaphore");
        return -1;
    }
    int NR_THREADS=4;
    int i;
    int param=0;
    pthread_t tid[NR_THREADS];
    for(i=0;i<NR_THREADS;i++)
    {
        pthread_create(&tid[i],NULL, thread_function, &param);
    }

     for(i=0;i<NR_THREADS;i++)
    {
        pthread_join(tid[i], NULL);
    }
    printf("%ld", count);
    sem_destroy(&semafor);

 }