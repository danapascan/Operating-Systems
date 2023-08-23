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
 char s[100]="Acesta este stringul transmis!";
 char s2[100];
 int lungimes1=strlen(s)-1;
 int lungimes2=0;

 if(pipe(fd) != 0) 
 {
    perror("Could not create pipe");
    return 1;
 }

 if(fork() != 0)
  {
    //parent
    close(fd[0]);
    write(fd[1], &lungimes1, sizeof(lungimes1));
    write(fd[1], s, sizeof(s));
    printf("Am citit sirul\n");
    close(fd[1]);
    wait(NULL);
 } 
 else 
 {
     //child
    close(fd[1]); 
    read(fd[0], &lungimes2, sizeof(lungimes2));
    read(fd[0], s2, lungimes2);
    printf("%s\n", s2);
    close(fd[0]);
 }

 return 0;

 }
 

