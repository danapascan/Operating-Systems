#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void *nimic(void* param)
{
    int i=0;
    int rez=0;
    for(i=0;i<10;i++)
    {
        rez+=i;
    }
    return (void*)(long)rez;
}

int main()
{
    pthread_t tid;
    int param=0;
    int nr_threads=0;

    //cat timp se mai poate face un thread
    while(pthread_create(&tid,NULL, nimic, &param)==0)
    {
        //pthread_join(tid, NULL);
        nr_threads++;
    }
    pthread_join(tid, NULL);
    printf("Numarul de thread-uri care se poate face este %d", nr_threads);
}