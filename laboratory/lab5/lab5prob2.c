#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void sterge(const char *path)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;

    dir=opendir(path);
    if(dir==NULL)
    {
        perror("Could not open directory");
        return;
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
                   //rmdir(fullPath);
                  // printf("Se va sterge %s\n", fullPath);
                   sterge(fullPath);
                }
                else if(S_ISREG(statbuf.st_mode))
                {
                    //printf("Se va sterge 1 %s\n" ,fullPath);
                    unlink(fullPath);
                }
            }
        }
    }
    //printf("Se va sterge 2 %s\n" ,path);
    rmdir(path);
    closedir(dir);

}

int main(int argc, char** argv)
{
    sterge("/home/dana/SO/test");
    return 0;
}
