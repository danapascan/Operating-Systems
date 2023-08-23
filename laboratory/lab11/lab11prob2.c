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

#define FIFO_NAME "my_fifo3"

int main(void)
 {  
    int fd =-1;

    if(mkfifo(FIFO_NAME, 0600)!=0)
    {
        perror("Err creating FIFO");
        return 1;
    }

    fd=open(FIFO_NAME, O_WRONLY);

    if(fd==-1)
    {
        perror("Could not open FIFO");
        return 1;
    }

    char c='p';
    int maxsizeanum=0;

    while(write(fd, &c, sizeof(c)))
    {
        maxsizeanum++;
        printf("%d\n",maxsizeanum);
    }

    close(fd);
    unlink(FIFO_NAME);
    return 0;
 }