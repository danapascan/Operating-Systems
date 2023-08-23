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
    pid_t pid=-1; 
	int status=0;

	char numar1[3];
	char numar2[3];
	char caracter[3];

	scanf("%s", numar1);
	scanf("%s", numar2);
	scanf("%s", caracter);
	
	pid = fork();

	if (pid == -1)
    {
		perror("Error creating new process");
		exit(1);
	}
	if (pid > 0) 
    { 	
		waitpid(pid, &status, 0);
		int rezultat=0;
		rezultat = WEXITSTATUS(status);
		printf("Rezultat: %d\n", rezultat);

	} 
    else if(pid==0)
    {
		execl("./server","server" ,numar1, numar2, caracter, NULL);
	}
	
	return 0;
}
