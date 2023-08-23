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

    fd=open(FIFO_NAME, O_RDONLY);

    if(fd==-1)
    {
        perror("Could not open FIFO");
        return 1;
    }


    close(fd);

    return 0;
 }