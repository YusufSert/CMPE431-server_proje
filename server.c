#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<stdbool.h>


void LIST(char *dir)
{
    DIR *directory = opendir(dir);
    struct dirent *sd;
    while ((sd = readdir(directory)) != NULL)
    {
        char buf[512];
        snprintf(buf, 512, "%s %i\n", sd->d_name, sd->d_reclen);
        printf("%s\n",buf);
    }


}
void GET(char filename[20])
{
    char buffer[512];
    FILE *fp = fopen(filename, "r");
    while (fgets(buffer, sizeof(buffer), fp ))
    {
        printf("%s",buffer);
    }

}


int main(int argc, char **argv) {
    char *dir;
    char *port;
    char *passFile;
    int login = 0;

    if(argc == 7){

        for(int i=0; i < argc; i++)
        {
            if(strcmp("-d",argv[i]) == 0){
                dir = argv[i+1];
                printf("%s\n", dir);
            }
            if(strcmp("-p",argv[i]) == 0){
                port = argv[i+1];
                printf("%s\n",port);
            }
            if(strcmp("-u",argv[i]) == 0){
                passFile = argv[i+1];
                printf("%s\n",passFile);
            }
        }
            login = 1;
    }
    else
    {
        printf("Give correct arguments");
    }

    if(login){
     LIST(dir);
     GET(passFile);
    }







    return 0;
}
