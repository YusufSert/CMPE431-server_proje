#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<stdbool.h>


int main(int argc, char **argv) {
    char *dir;
    char *port;
    char *passFile;
    int x = 0;

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
            x = 1;
    }
    else
    {
        printf("Give correct arguments");
    }

    if(x){
        printf("arguments are okişş");
    }







    return 0;
}
