#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <dirent.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_NAME1 "RESP_PIPE_80818"
#define PIPE_NAME2 "REQ_PIPE_80818"
#define MEM_NAME "/NFAhA7tJ"

int main()
{
     if(mkfifo(PIPE_NAME1,0600)!=0)
     {
        printf("ERROR\n");
        printf("Cannot create the response pipe");
        return 1;
     }

     int fd1=-1;
     int fd2=-1;
     char *mesaj="BEGIN#";

     fd2=open(PIPE_NAME2, O_RDONLY);
     if(fd2==-1)
     {
        printf("ERROR\n");
        printf("Cannot open the request pipe");
        return 1;
     }

     fd1=open(PIPE_NAME1, O_WRONLY);
     if(fd1==-1)
     {
        printf("ERROR\n");
        printf("Cannot open the response pipe");
        return 1;
     }

     write(fd1, mesaj, 6);
     printf("SUCCESS");

     int mem=-1;
     mem=shm_open(MEM_NAME, O_CREAT |O_RDWR, 0664);
     char *data;

     for(;;)
     {
       int index=0;
       char req[200];
       while(read(fd2, &req[index], 1)>0)
       {
            if(req[index]=='#')
            {
               break;
            }
            index++;
       }
       req[++index]='\0';
       if(strcmp(req, "VARIANT#")==0)
       {
         write(fd1, "VARIANT#", 8);
         unsigned int numar=80818;
         write(fd1,&numar,sizeof(unsigned int) );
         write(fd1, "VALUE#", 6);
       }
       else if(strcmp(req, "CREATE_SHM#")==0)
       {
            unsigned int numar2=0;
            read(fd2, &numar2, sizeof(unsigned int));
            if(mem!=-1)
            {
               int trunc=ftruncate(mem,4499996);
               if(trunc==-1)
               {
                     write(fd1, "CREATE_SHM#", 11);
                     write(fd1, "ERROR#", 6);
               }
               else
               {
                     data=(char*)mmap(NULL,4499996, PROT_WRITE , MAP_SHARED, mem,0);

                     if(data==(void*)-1)
                     {
                        write(fd1, "CREATE_SHM#", 11);
                        write(fd1, "ERROR#", 6);
                        close(mem);
                     }
                     else
                     {
                        write(fd1, "CREATE_SHM#", 11);
                        write(fd1, "SUCCESS#", 8);
                     }
               }
            }
            else{
               write(fd1, "CREATE_SHM#", 11);
               write(fd1, "ERROR#", 6);
            }
       }
       else if(strcmp(req, "WRITE_TO_SHM#")==0)
       {
            unsigned int offset=0;
            unsigned int value=0;
            read(fd2, &offset, sizeof(unsigned int));
            read(fd2, &value, sizeof(unsigned int));

            if(offset>=0 && (offset+3)<4499996)
            {
                     memcpy(data+offset, &value, 4);
                     write(fd1, "WRITE_TO_SHM#", 13);
                     write(fd1, "SUCCESS#", 8);
            }
            else{
               write(fd1,"WRITE_TO_SHM#", 13);
               write(fd1, "ERROR#", 6);
               close(mem);
               
            }
       }
       else if(strcmp(req, "MAP_FILE#")==0)
       {
            char fisier[200];
            int i=0;
            while(read(fd2, &fisier[i], 1)>0)
            {
               if(fisier[i]=='#')
               {
                  break;
               }
               i++;
            }
            fisier[i]='\0';
            int fd3=-1;
            fd3=open(fisier, O_RDONLY);
            if(fd3==-1)
            {
              write(fd1,"MAP_FILE#", 9);
               write(fd1, "ERROR#", 6);
            }
            int size=0;
            size=lseek(fd3, 0, SEEK_END);
            lseek(fd3, 0, SEEK_SET);
            data=(char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd3, 0);

            if(data==(void*)-1)
            {
               write(fd1,"MAP_FILE#", 9);
               write(fd1, "ERROR#", 6);
            }
            else
            {
                write(fd1, "MAP_FILE#", 9);
                write(fd1, "SUCCESS#", 8);
            }
       }
       else 
       {
         break;
       }
     }

      close(fd1);
      close(fd2);
      unlink(PIPE_NAME1);
      
      
      unlink(PIPE_NAME2);

    return 0;
}
