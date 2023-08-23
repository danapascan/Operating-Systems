#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 64

int main(int argc, char** argv)
{
  int fd1=-1;
   int fd2=-1;
   int size =0;

   fd1= open("p2l4.txt", O_RDONLY);
  if(fd1<0)
  {
     perror("Cannot open the file");
     exit(-1);
   }

   fd2= open("p2l4copy.txt", O_WRONLY);
  if(fd2<0)

  {
     perror("Cannot open the file");
     exit(-1);
   }
   
   size= lseek(fd1,-1, SEEK_END);
   
   char aux=0;
   
   while(size!=-1)
   { 
         if(read(fd1,&aux,1 )!=1)
         {
            return -1;
         }
          
         if(write(fd2,&aux, 1)!=1)
         {
           return -1;
           
           }
          printf("%c", aux);
          size=lseek(fd1, -2, SEEK_CUR);
          size--;
   }
   
   close(fd1);
   close(fd2);
   
  return 0;
  }
  
