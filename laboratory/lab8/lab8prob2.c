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
#define n 16
int nrThreads=0;
sem_t semafor;


 void *limited_area(void *unused)
 {
    sem_wait( &semafor);
    nrThreads++;
    usleep(100);
    printf("The number of threads in the limited area is: %d\n", nrThreads);
    nrThreads--;
    sem_post(&semafor);
    return NULL;
 }

 int main(int argc,char * argv[])
 {
    int p=atoi(argv[1]);
    sem_init(&semafor,0,p);
    int i;
    int param=0;
    pthread_t tid[n];
    for(i=0;i<n;i++)
    {
        pthread_create(&tid[i],NULL,limited_area, &param);
    }

     for(i=0;i<n;i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&semafor);
 }