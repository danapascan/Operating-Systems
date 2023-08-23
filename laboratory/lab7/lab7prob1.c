#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

typedef struct{
int *arr;
int from;
int to;
}THREAD_STRUCT;

#define NR_THREADS 4

void *nr_unu(void*param)
{
    int count=0;
    int i=0;
    THREAD_STRUCT *th=(THREAD_STRUCT*)param;

    //numaram pentru fiecare element din arr cate cifre de 1 are si adaugam la count
    for(i=th->from;i<=th->to;i++)
    {
        int aux=0;
        while(th->arr[i]!=0)
        {
            aux=th->arr[i]%10;
            if(aux==1)
            {
                count++;
            }
            th->arr[i]=th->arr[i]/10;
        }
    }
    return (void*)(long)count;
}


int main()
{
    int nr=0;
    scanf("%d", &nr);
    int SIZE_ARR=nr+1;
    pthread_t tid[NR_THREADS];
    void * rezultat;
    THREAD_STRUCT param[NR_THREADS];
    int i=0;

    int th_rez_final=0;
    int rezultat_final=0;

    //facem un vector cu numerele de la 0 la nr-ul citit de la
    //tastatura pentru a le adauga in structura la fiecare thread
    int v[nr+1];
    for(i=0;i<=nr;i++)
    {
        v[i]=i;
    }

    //aici impartim munca echitabil pentru fiecare thread
    for(int i=0;i<NR_THREADS;i++)
    {
        param[i].arr=v;
        if(i==0)
        {
            param[i].from=0;
        }
        else
        {
            param[i].from=param[i-1].to+1;
        }
        param[i].to=param[i].from+ SIZE_ARR/NR_THREADS-1;
        //in cazul in care impratirea la thread uri nu se face exact 
        //mai adaugam cate un element in plus 
        if(i<SIZE_ARR%NR_THREADS)
        {
            param[i].to++;
        }
    }

    //aici se creeaza threadurile
    for(i=0;i<NR_THREADS;i++)
    {
        pthread_create(&tid[i],NULL, nr_unu, &param[i]);
    }

    //aici dam join la fiecare thread in parte
    for(i=0;i<NR_THREADS;i++)
    {
        pthread_join(tid[i],&rezultat);
        th_rez_final=(int)(long)rezultat;
        //aici adaugam la suma totala rezultatul din thread
        rezultat_final+=th_rez_final;
    }

    printf("Numarul de cifre de 1 este egal cu %d", rezultat_final);
    return 0;
}