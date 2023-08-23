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
 
int n=10;
int v[10]={14,28,55,34,11,62,67,92,35,18};
int nrDiv[10];
int nextTask=0;

typedef  struct 
{
    int id;
    pthread_mutex_t *lock;
}TH_STRUCT;

void divizorinr(int index)
{
    int nrdivizori=0;
    for(int i=1;i<=v[index];i++)
    {
        if(v[index]%i==0)
        nrdivizori++;
    }
    nrDiv[index]=nrdivizori;

}

void *divizorithreads(void * arg)
{
    TH_STRUCT *s = (TH_STRUCT*)arg;
    int myTask=0;

    for(;;)
    {   
        pthread_mutex_lock(s->lock);
        myTask=nextTask;
        nextTask++;
        divizorinr(myTask);
        pthread_mutex_unlock(s->lock);
        if(nextTask==n)
        {
            break;
        }
    }
    pthread_exit(NULL);

}

int main(int argc,char * argv[])
{
 int nr;
 nr=atoi(argv[1]);
 //printf("%d ", nr);
 TH_STRUCT params[nr];
 pthread_t tids[nr];
 pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

 int i;
 for(i=0;i<nr;i++)
 {
    params[i].id=i;
    params[i].lock= &lock;
    pthread_create(&tids[i], NULL, divizorithreads, &params[i]);
 }

 for(i=0; i<nr; i++) 
 {
    pthread_join(tids[i], NULL);
 }

 pthread_mutex_destroy(&lock);

    for(int i=0;i<n;i++)
    {
        printf("%d ", nrDiv[i]);
    }
    
 return 0;
}