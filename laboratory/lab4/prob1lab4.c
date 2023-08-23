#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int get_line(int fd, int lineNr, char *line, int maxLength)
{
   int i;
   int nrlinii=0;
   int index=0;
   char * buffer=(char*) malloc(maxLength);
   if(lineNr==1)
   {
     while(read(fd, &line[i],1)>0)
            {
               if (line[i]!='\n')
               {
                 i++;
               }
                  else break;
                  }
   }
   else
   {
   while((index<maxLength) && (read(fd, &buffer[index],1)>0))
   {
     
      if(buffer[index]=='\n')
      {
         nrlinii++;
         if(nrlinii==lineNr-1)
         { 
            i=0;
            while(read(fd, &line[i],1)>0)
            {
               if (line[i]!='\n')
               {
                 i++;
               }
                  else break;
            }
          }
        }
      }
      }
   line[i]=0;
   return 0;
  } 
  
int main(int argc, char** argv)
{
  int maxLength= 1024;
  char *line=(char*)malloc(maxLength);
  int fd;
  fd= open("p1l4.txt", O_RDONLY);
  if(fd<0)
  {
     perror("Cannot open the file");
     exit(-1);
   }
   
  printf("%d\n",get_line(fd, 2,line, maxLength));
  printf("%s", line);
  close(fd);
  
    return 0;
}
