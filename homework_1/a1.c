#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void afisarecontinutrecursiv(const char *path, int size, const char* end)
{

    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;

    dir=opendir(path);

    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
        {
            
            snprintf(fullPath, 512, "%s/%s",path, entry->d_name);
            if(lstat(fullPath,&statbuf)==0)
            {   
                if(size!=0)
                {
                    if(S_ISREG(statbuf.st_mode) && statbuf.st_size<size)
                    {
                        printf("%s\n", fullPath);
                    }
                     if(S_ISDIR(statbuf.st_mode))
                    {
                        afisarecontinutrecursiv(fullPath, size, end);
                    }
                }
                 else if(end!=NULL)
                {
                    char * foundstring=NULL;
                    foundstring=strstr(entry->d_name, end);
                    if(S_ISREG(statbuf.st_mode) && foundstring!=NULL && strcmp(foundstring, end)==0)
                      { 
                        printf("%s\n", fullPath);
                      }
                       else if(foundstring!=NULL)
                     {
                        strcpy(foundstring, foundstring+(strlen(end)+1));
                        if(strcmp(foundstring, end)==0)
                        {
                            printf("%s\n", fullPath);
                        }
                     }
                    
                     if(S_ISDIR(statbuf.st_mode))
                     {
                       afisarecontinutrecursiv(fullPath, size, end);
                     }
                     
                }
                else if(size==0 && end==NULL)
                {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode))
                {
                    afisarecontinutrecursiv(fullPath, size, end);
                }
                }
            }
            
        }
    }
    closedir(dir);
}


void afisarecontinutsimplu(const char * path, int size, char* end)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;
    dir=opendir(path);
    if(dir==NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    } 
    else 
    printf("SUCCESS\n");

     while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
        {
            snprintf(fullPath, 512, "%s/%s",path, entry->d_name);
            if(lstat(fullPath,&statbuf)==0)
            { 
                if(size!=0)
                {
                  if(S_ISREG(statbuf.st_mode)&&statbuf.st_size<size)
                  {
                    printf("%s\n", fullPath);
                  }
                }
                else if(end!=NULL)
                {
                     char * foundstring=NULL;
                     foundstring=strstr(entry->d_name, end);
                     if(foundstring!=NULL && strcmp(foundstring, end)==0)
                        printf("%s\n", fullPath);
                     else if(foundstring!=NULL)
                     {
                        strcpy(foundstring, foundstring+(strlen(end)+1));
                        if(strcmp(foundstring, end)==0)
                        {
                            printf("%s\n", fullPath);
                        }
                     }
                }
                else if (size==0 && end==NULL)
                {
                    printf("%s\n", fullPath);
                }

            }
        }
    }
    if(end!=NULL)
     {
        char * foundstring=NULL;
        foundstring=strstr(path, end);
        if(foundstring!=NULL && strcmp(foundstring, end)==0)
        printf("%s\n", path);   
    }

    closedir(dir);

}

int parse(char * path)
{
    int fd1=-1;
    char magic[3];

    fd1= open(path, O_RDONLY);
    if(fd1<0)
    {
     printf("Cannot open the file");
     close(fd1);
     return -1;
    }

    lseek(fd1,0,SEEK_SET);
    read(fd1,magic,2);
    magic[2]=0;
    
    if(strcmp(magic, "DC")!=0)
    {
        printf("ERROR\nwrong magic");
        close(fd1);
        return -1;
    }

    lseek(fd1,2,SEEK_CUR);
    int version=0;
    read(fd1, &version,1);
    if(version<38 || version>150)
    {
        printf("ERROR\nwrong version");
        close(fd1);
        return -1 ;
    }
     
    int nr_sect=0;
    read(fd1, &nr_sect,1 );
    if(nr_sect<8 || nr_sect>16)
    {
        printf("ERROR\nwrong sect_nr");
        close(fd1);
        return -1;
    }
    
    char sect_name[nr_sect][19];
    int sect_type[nr_sect];
    int sect_offset[nr_sect];
    int sect_size[nr_sect];

    for(int i=0;i<nr_sect;i++)
    {
        sect_type[i]=0;
        sect_offset[i]=0;
        sect_size[i]=0;
    }
    for(int i=0;i<nr_sect;i++)
    {
        char name[19]="";
        read(fd1, name,18);
        sprintf(sect_name[i], "%s", name);
        name[18]=0;
        read(fd1,&sect_type[i],1);
        if(sect_type[i]!=77 && sect_type[i]!=31 && sect_type[i]!=26 && sect_type[i]!=94 && sect_type[i]!=90)
        {
            printf("ERROR\nwrong sect_types");
            close(fd1);
            return -1;
        }
        else{
        read(fd1,&sect_offset[i],4);
        read(fd1, &sect_size[i],4);
        }
    }
    printf("SUCCESS\n");
    printf("version=%d\n", version);
    printf("nr_sections=%d\n",nr_sect);

    for(int i=0;i<nr_sect;i++)
    {
        printf("section%d: ",i+1);
        printf("%s ", sect_name[i]);
        printf("%d %d\n", sect_type[i],sect_size[i]);
        
    }
    close(fd1);
    return 0;
}


int extract(char * path , int section, int linie )
{
    int fd2=-1;
    char magic[3];

    fd2= open(path, O_RDONLY);
    if(fd2<0)
    {
     printf("invalid file");
     close(fd2);
     return -1;
    }

    lseek(fd2,0,SEEK_SET);
    read(fd2,magic,2);
    magic[2]=0;
    
    if(strcmp(magic, "DC")!=0)
    {
        printf("ERROR\ninvalid file");
        close(fd2);
        return -1;
    }

    lseek(fd2,2,SEEK_CUR);
    int version=0;
    read(fd2, &version,1);
    if(version<38 || version>150)
    {
        printf("ERROR\ninvalid file");
        return -1 ;
    }
     
    int nr_sect=0;
    read(fd2, &nr_sect,1 );
    if(nr_sect<8 || nr_sect>16)
    {
        printf("ERROR\ninvalid file");
        close(fd2);
        return -1;
    }
    
    char sect_name[nr_sect][19];
    int sect_type[nr_sect];
    int sect_offset[nr_sect];
    int sect_size[nr_sect];

    for(int i=0;i<nr_sect;i++)
    {
       sect_type[i]=0;
        sect_offset[i]=0;
        sect_size[i]=0;
    }
    for(int i=0;i<nr_sect;i++)
    {
        char name[19]="";
        name[18]=0;
        read(fd2, name,18);
        sprintf(sect_name[i], "%s", name);
        read(fd2,&sect_type[i],1);
        if(sect_type[i]!=77 && sect_type[i]!=31 && sect_type[i]!=26 && sect_type[i]!=94 && sect_type[i]!=90)
        {
            printf("ERROR\ninvalid file");
            close(fd2);
            return -1;
        }
        else{
        read(fd2,&sect_offset[i],4);
        read(fd2, &sect_size[i],4);
        }
    }

    
       int index=0;
       int max_length=2000;
       int totallength=0;
       char line[2000][max_length];
       char aux[2000][max_length];
       int i=0;
       int nrlinii=0;

       if(section>nr_sect)
       {
        printf("ERROR\ninvalid section");
        close(fd2);
         return -1;
       }
       else{

                lseek(fd2, 0, SEEK_SET);
                lseek(fd2,sect_offset[section-1],SEEK_SET);
                while( totallength <= sect_size[section-1] && read(fd2, &aux[index][i],1)>0)
                {
                    if(aux[index][i]=='\n')
                    {
                        aux[index][i+1]='\0';
                        sprintf(line[index], "%s", aux[index]);
                        index++;
                        i=0;
                        nrlinii++;
                        totallength++;
                    }
                    else 
                    {
                        i++;
                        totallength++;
                    }
                }
                sprintf(line[index], "%s", aux[index]);
            }

    //printf("SUCCESS\n"); 
    if(linie>nrlinii)
    {
        printf("ERROR\ninvalid line");
        close(fd2);
        return -1;
    }
    else
        {
            printf("SUCCESS\n"); 
            printf("%s", line[nrlinii-linie+1]);
        }

    close(fd2);
    return 0;
}

int findall(char * path)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;

    dir=opendir(path);

    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0)
        {
            
            snprintf(fullPath, 512, "%s/%s",path, entry->d_name);
            if(lstat(fullPath,&statbuf)==0)
            {
                        int fd1=-1;
                        char magic[3];
                        int ok=1;

                        fd1= open(fullPath, O_RDONLY);
                        if(fd1<0)
                        {
                                ok=0;
                        }

                        lseek(fd1,0,SEEK_SET);
                        read(fd1,magic,2);
                        magic[2]=0;
    
                        if(strcmp(magic, "DC")!=0)
                        {
                                ok=0;
                        }

                        lseek(fd1,2,SEEK_CUR);
                        int version=0;
                        read(fd1, &version,1);

                        if(version<38 || version>150)
                        {
                                ok=0;
                        }
     
                        int nr_sect=0;
                        read(fd1, &nr_sect,1 );
                        if(nr_sect<8 || nr_sect>16)
                        {
                                ok=0;
                        }
    
                        char sect_name[nr_sect][19];
                        int sect_type[nr_sect];
                        int sect_offset[nr_sect];
                        int sect_size[nr_sect];

                        for(int i=0;i<nr_sect;i++)
                        {
                                sect_type[i]=0;
                                sect_offset[i]=0;
                                sect_size[i]=0;
                        }
                        for(int i=0;i<nr_sect;i++)
                        {
                                char name[19]="";
                                
                                read(fd1, name,18);
                                sprintf(sect_name[i], "%s", name);
                                name[18]=0;
                                read(fd1,&sect_type[i],1);
                                if(sect_type[i]!=77 && sect_type[i]!=31 && sect_type[i]!=26 && sect_type[i]!=94 && sect_type[i]!=90)
                                {
                                    ok=0;
                                }
                                else{
                                read(fd1,&sect_offset[i],4);
                                read(fd1, &sect_size[i],4);
                                }
                        }
                
                        if(ok)
                        {
                            int sectiune=0;
                            int nrminlinii=0;
      
                            while(sectiune<nr_sect)
                            {
                                lseek(fd1,sect_offset[sectiune],SEEK_SET);

                                int nrlinii=0;
                                int totallength=0; 
                                char c=0;
                   
                                while(totallength<=sect_size[sectiune] && read(fd1, &c,1)>0)
                                {
                                    if(c=='\n')
                                    {
                                        nrlinii++;
                                        totallength++;
                                    }
                                    else 
                                    {
                                        totallength++;
                                    }
                                }
                                c=0;
                                if(nrlinii==14)
                                {
                                    nrminlinii++;
                                }
                                sectiune++;
                
                            }

                            if (nrminlinii>=2)
                            {
                                printf("%s\n", fullPath);
                            }
                        }
                        
                        close(fd1);

                if(S_ISDIR(statbuf.st_mode))
                {
                    findall(fullPath);
                }
                
            }
        
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv){
  
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0){
            printf("80818\n");
        }
    }

    if(argc>=3)
    {

        char *pathstring=NULL;
        for(int i=1;i<argc;i++)
        {
            if(strstr(argv[i], "path")!=0)
            {
                pathstring=argv[i];
            }
        }
        int listok=0;
        int findallok=0;
        int extractok=0;
        int parseok=0;

        for(int i=1;i<argc;i++)
        {
            if(strcmp(argv[i],"list")==0)
            {
                listok=1;
            }
            if(strcmp(argv[i],"findall")==0)
            {
                findallok=1;
            }
            if(strcmp(argv[i],"extract")==0)
            {
                extractok=1;
            }
            if(strcmp(argv[i],"parse")==0)
            {
                parseok=1;
            }
        }
        if(listok==1)
        {
            for(int i=1;i<argc;i++)
            {
                //aici aven list si recursive si path
                if(strcmp(argv[i],"recursive")==0)
                {
                    int ok=0;
                   // if(argc>3)
                   // {
                        for(int i=1;i<argc;i++)
                        {
                            //verificam daca avem criteriu de nume
                            if(argv[i][0]=='n')
                            {
                                char *nume=NULL;
                                int size=0;
                                nume=argv[i];
                                DIR *dir=NULL;
                                dir=opendir(pathstring+5);
                                ok=1;
                                if(dir==NULL)
                                {
                                    printf("ERROR\ninvalid directory path\n");
                                }
                                else
                                { 
                                    printf("SUCCESS\n");
                                    afisarecontinutrecursiv(pathstring+5, size, nume+15);
                                }
                                closedir(dir);
                            }
                            //verificam daca avem criteriu de size
                            if(argv[i][0]=='s')
                            {
                                char *nume=NULL;
                                int size=0;
                                char* sizestring=NULL;
                                sizestring= argv[i];
                                size= atoi(sizestring+13);
                                DIR *dir=NULL;
                                dir=opendir(pathstring+5);
                                ok=1;
                                if(dir==NULL)
                                {
                                    printf("ERROR\ninvalid directory path\n");
                                }
                                else
                                { 
                                    printf("SUCCESS\n");
                                    afisarecontinutrecursiv(pathstring+5, size, nume);
                                }
                                closedir(dir);
                            }
                        }
                        if(ok==0)
                        {
                            char *nume=NULL;
                            int size=0;
                            DIR *dir=NULL;
                            dir=opendir(pathstring+5);
                            if(dir==NULL)
                            {
                                printf("ERROR\ninvalid directory path\n");
                            }
                            else
                            { 
                                printf("SUCCESS\n");
                                afisarecontinutrecursiv(pathstring+5, size, nume);
                            }                                
                            closedir(dir);
                        }
                } 
            }
            //avem doar list
            if(argc==4)
            {
                for(int i=1;i<argc;i++)
                {
                    //verificam daca avem criteriu de nume
                    if(argv[i][0]=='n')
                    {
                        char *nume=NULL;
                        int size=0;
                        nume=argv[i];
                        afisarecontinutsimplu(pathstring+5, size, nume+15);
                    }
                    //verificam daca avem criteriu de size
                    if(argv[i][0]=='s')
                    {
                        char *nume=NULL;
                        int size=0;
                        char* sizestring=NULL;
                        sizestring= argv[i];
                        size= atoi(sizestring+13);
                        afisarecontinutsimplu(pathstring+5, size, nume);
                    }
                }
            }
            else if(argc==3)
            {
                char *nume=NULL;
                int size=0;
                afisarecontinutsimplu(pathstring+5, size, nume);
            }
        }
        if(extractok==1)
        {
            char * sec=NULL;
            char * line=NULL;
            for(int i=1;i<argc;i++)
            {
                if(argv[i][0]=='s')
                {
                    sec=argv[i];
                }
                if(argv[i][0]=='l')
                {
                    line=argv[i];
                }
            }
            int section=atoi(sec+8);
            int linie=atoi(line+5);
            extract(pathstring+5, section, linie);
        }
        if(findallok==1)
        {
            DIR *dir=NULL;
            dir=opendir(pathstring+5);
            if(dir==NULL)
            {
                printf("ERROR\ninvalid directory path\n");
            }
            else
            { 
                printf("SUCCESS\n");
                findall(pathstring+5);
            }
            closedir(dir);
        }
        if(parseok==1)
        {
            parse(pathstring+5);
        }
    }
    return 0;
}

