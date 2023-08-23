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

int main(int argc, char **argv)
{
  pid_t pid2 = -1, pid3 = -1, pid4 = -1, pid5 = -1;

  pid2 = fork();

  if (pid2 == -1)
  {
    perror("Cannot create child");
    return -1;
  }
  else if (pid2 == 0)
  {
    pid4 = fork();
    if (pid4 == -1)
    {
      perror("Cannot create child");
      return -1;
    }
    else if (pid4 == 0)
    {
      printf("pid: %d , father: %d\n", getpid(), getppid());
       sleep(60);
    }
    else
    {
      waitpid(pid4, NULL, 0);
      printf("pid :%d , father: %d\n", getpid(), getppid());
    }
  }
  else if (pid2 > 0)
  {
    printf("My pid is:%d\n", getpid());
    pid3 = fork();
    if (pid3 == -1)
    {
      perror("Cannot create child");
      return -1;
    }
    else if (pid3 == 0)
    {
      pid5 = fork();
      if (pid5 == 0)
      {
        printf("pid: %d , father: %d\n", getpid(), getppid());
         sleep(60);
      }
      else
      {
        waitpid(pid5, NULL, 0);
        printf("pid :%d , father: %d\n", getpid(), getppid());
      }
    }
    else
    {
      waitpid(pid2,NULL,0);
      waitpid(pid3, NULL,0);
      printf("pid: %d\n", getpid());
    }
  }

  return 0;
}
