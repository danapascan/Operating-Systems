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

    int numar1=0;
    int numar2=0;

    numar1 = atoi(argv[1]);
	numar2 = atoi(argv[2]);

    int rezultat=0;
	if(strcmp(argv[3],"+")==0)
	{
            rezultat=numar1+numar2;
    }
	else if(strcmp(argv[3],"-")==0)
    {
		rezultat=numar1-numar2;
    }

	exit(rezultat);

	return 0;

}
