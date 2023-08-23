#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

off_t dirSize(const char *path)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;
    off_t size=0;

    dir=opendir(path);
    if(dir==NULL)
    {
        perror("Could not open directory");
        return 0;
    }
    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
        {
            snprintf(fullPath, 512, "%s/%s",path, entry->d_name);
            if(lstat(fullPath,&statbuf)==0)
            {
                //printf("%s\n",fullPath);
                if(S_ISDIR(statbuf.st_mode))
                {
                    size=size+dirSize(fullPath); 
                }
                else if(S_ISREG(statbuf.st_mode))
                {
                    size+=statbuf.st_size;
                }

            }
        }
    }
    closedir(dir);
    return size;
}

int main(int argc, char** argv)
{
    off_t size;
    size=dirSize("/home/dana/SO");
    printf("%ld", size);
    return 0;
}
