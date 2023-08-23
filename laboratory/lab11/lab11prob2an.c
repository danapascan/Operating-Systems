#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
 {

 int fd[2];

 if(pipe(fd) != 0) 
 {
    perror("Could not create pipe");
    return 1;
 }

    char c='p';
    int maxan=0;

 if(fork() != 0)
  {
    //parent
    close(fd[0]);
    while(write(fd[1], &c, sizeof(c)))
    {
        maxan++;
        printf("%d\n", maxan);
    }
    close(fd[1]);
    wait(NULL);
 } 
 else 
 {
     //child
    close(fd[1]); 
    while(1)
    {

    }
    close(fd[0]);
 }

 return 0;

 }
 

