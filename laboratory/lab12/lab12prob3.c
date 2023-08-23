#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


void make_file(char* data, size_t size) 
{
    size_t i, j;
    for (i = 0, j = 0; i < size; i++) {
        if (!(data[i] == 'a' || data[i] == 'e' || data[i] == 'i' || data[i] == 'o' || data[i] == 'u'))
        {
            data[j++] = data[i];
        }
    }
    
   for (int i=j;i<size;i++) 
    {
        data[i] = ' ';
    }
   
}

int main(int argc, char **argv) {
    int fd;
    off_t size;
    char *data = NULL;

    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
    return 1;
    }

    fd = open(argv[1], O_RDWR);
    if(fd == -1) 
    {
        perror("Could not open input file");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);

    data = (char*)mmap(NULL, size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

     if(data==(void*)-1)
    {
        perror("Could not map file");
        close(fd);
        return 1;
    }

    make_file(data,size);
    munmap(data, size);
    close(fd);

    return 0;
}