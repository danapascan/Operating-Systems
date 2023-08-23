#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "a2_helper.h"
int NR_THREADS=9;
int curnrth=0;
int ok=0;
typedef struct 
{
    int process;
    int thread;
    pthread_mutex_t *lock1, *lock2, *lock3, *lock4, *lock5;
    sem_t *semaphore, * semaphore2;
}TH_STRUCT;

/*pentru cele 2 probleme cu sincronizare de thread-uri*/
void *syncronize(void* args)
{
    TH_STRUCT *param=(TH_STRUCT*)args;

    /*thread ul 4 trebuie sa inceapa inainte ca thread-ul 1 sa inceapa
 deci il blocam pe 1*/ 

    if(param->thread== 1 && param->process==5)
    {
        pthread_mutex_lock(param->lock1);
    }
    if(param->thread==3 && param->process==5)
    {
        pthread_mutex_lock(param->lock3);
    }
    /*thread-ul 4.4 nu poate incepe decat dupa ce thread-ul 5.3 s-a terminat*/

    if(param->thread==4 && param->process==4)
    {
        pthread_mutex_lock(param->lock2);
    }
    info(BEGIN, param->process, param->thread);
    /*thread-ul 5.4 trebuie sa se incheie dupa 5.1*/

    if(param->thread==4 && param->process==5)
    {
        pthread_mutex_unlock(param->lock1);
    }
    if(param->thread==4 && param->process==5)
    {
        pthread_mutex_lock(param->lock4);
    }
    info(END, param->process, param->thread);
    /*threadul T4.1 trebuie sa se inchece inainte ca T5.3 saporneasca*/
    if(param->thread==1 && param->process==4)
    {
        pthread_mutex_unlock(param->lock3);
    }
    if(param->thread==3 && param->process==5)
    {
        pthread_mutex_unlock(param->lock2);
    }
    if(param->thread==1 && param->process==5)
    {
        pthread_mutex_unlock(param->lock4);
    }
    return NULL;
}
void *bar_thread(void * args)
{
    TH_STRUCT *param = (TH_STRUCT *)args;
    sem_wait(param->semaphore);
    if(param->thread!=15)
    { 
        sem_wait(param->semaphore2);

        info(BEGIN, param->process, param->thread);
        curnrth++;
        if(curnrth==6 && ok==0)
        {
          
            sem_wait(param->semaphore2); 
            info(BEGIN,param->process, 15);
            info(END, param->process, 15);
            ok=1;
            sem_post(param->semaphore2);
        }
        sem_post(param->semaphore2);
        sem_wait(param->semaphore2);
        info(END, param->process, param->thread);
        sem_post(param->semaphore2);
        
    }
    sem_post(param->semaphore);
    
    return NULL;
}

/*creem thread-uri pentru problema 3*/
void create_th(pthread_t tid[], int NR_THREADS, TH_STRUCT* param)
{
    int i;
    for(i=1;i<=NR_THREADS;i++)
    {
         pthread_create(&tid[i],NULL,syncronize,&param[i]);
    }
    for(i=1;i<=NR_THREADS;i++)
    {
        pthread_join(tid[i], NULL);
    }
}

/*creem thread-uri pentru problema 4*/
void create_th_p4(pthread_t tid[], int NR_THREADS, TH_STRUCT* param)
{
    int i;
    for(i=1;i<=NR_THREADS;i++)
    {
         pthread_create(&tid[i],NULL,bar_thread,&param[i]);
    }
    for(i=1;i<=NR_THREADS;i++)
    {
        pthread_join(tid[i], NULL);
    }
}


int main(){
    pid_t  pid2=-1, pid3=-1, pid4=-1, pid5=-1, pid6=-1, pid7=-1, pid8=-1;
    init();
    info(BEGIN, 1, 0);
    pid2=fork();
    if(pid2==0)
    {
        info(BEGIN, 2, 0);
        pid3=fork();
        if(pid3==0)
        {
            info(BEGIN, 3, 0);
            /*pentru problema 4*/
            /*procesul trebuie sa creeze 46 de treaduri*/
            pthread_t tid2[46];
            TH_STRUCT param2[46];
            sem_t sem, sem2;
            int i;
            for(i=1;i<=46;i++)
            {
                param2[i].thread=i;
                param2[i].process=3;
                param2[i].semaphore=&sem;
                param2[i].semaphore2=&sem2;
            }
            sem_init(&sem, 0, 6);
            sem_init(&sem2, 0, 6);
            create_th_p4(tid2, 46, param2);

            sem_destroy(&sem);
            sem_destroy(&sem2);

            info(END, 3, 0);
        }
        else if(pid3>0)
        {
            pid7=fork();
            if(pid7==0)
            {
                info(BEGIN, 7, 0);
                info(END, 7, 0);
            }
            else if(pid7>0)
            {
                waitpid(pid3, NULL, 0);
                waitpid(pid7, NULL, 0);
                info(END, 2, 0);
            }
            
        }
        
    }
    else if(pid2>0)
    {
        pid4=fork();
        if(pid4==0)
        {
            info(BEGIN, 4, 0);
            pid5= fork();
            if(pid5==0)
            {
                info(BEGIN, 5, 0);
                /*pentru problema cu sincronizarea threadurilor*/
                pthread_t tid[NR_THREADS+1];
                TH_STRUCT param[NR_THREADS];
                int i=0;
                
                pthread_mutex_t lock1= PTHREAD_MUTEX_INITIALIZER;
                pthread_mutex_t lock2= PTHREAD_MUTEX_INITIALIZER;
                pthread_mutex_t lock3= PTHREAD_MUTEX_INITIALIZER;
                pthread_mutex_t lock4= PTHREAD_MUTEX_INITIALIZER;
                pthread_mutex_t lock5= PTHREAD_MUTEX_INITIALIZER;

                for(i=1;i<=NR_THREADS;i++)
                {
                    if(i<=4)
                    {
                        param[i].thread=i;
                        param[i].process=4;
                    }
                    else if(i>4)
                    {
                        param[i].thread=i-4;
                        param[i].process=5;
                    }
                
                    param[i].lock1=&lock1;
                    param[i].lock2=&lock2;
                    param[i].lock3=&lock3;
                    param[i].lock4=&lock4;
                    param[i].lock5=&lock5;
                }
                pthread_mutex_lock(&lock1);
                pthread_mutex_lock(&lock2);
                pthread_mutex_lock(&lock3);
                pthread_mutex_lock(&lock4);
                pthread_mutex_lock(&lock5);

                create_th(tid, NR_THREADS+1, param);

                pthread_mutex_destroy(&lock1);
                pthread_mutex_destroy(&lock2);
                pthread_mutex_destroy(&lock3);
                pthread_mutex_destroy(&lock4);
                pthread_mutex_destroy(&lock5);

                pid6=fork();
                if(pid6==0)
                {
                    info(BEGIN, 6, 0);
                    pid8=fork();
                    if(pid8==0)
                    {
                        info(BEGIN, 8, 0);
                        info(END, 8, 0);
                    }
                    else if( pid8>0)
                    {
                        waitpid(pid8, NULL, 0);
                        info(END, 6, 0);
                    }
                }
                else if(pid6>0)
                {
                    waitpid(pid6, NULL, 0);
                    info(END, 5, 0);
                }
            }
            else if (pid5>0)
            {
                waitpid(pid5, NULL, 0);
                info(END, 4, 0);
            }
        }
        else if(pid4>0)
        {
            waitpid(pid2, NULL, 0); 
            waitpid(pid4, NULL, 0);
            info(END, 1, 0);
        }
    }
    
    return 0;
}
