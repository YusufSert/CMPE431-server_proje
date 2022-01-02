/* C header files */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<stdbool.h>
#include <sys/types.h>
#include <stdlib.h>

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>





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
void USER(char name[20], char password[20])
{
    char fileName[20], filePassword[20];
    FILE *fp;
    fp = fopen("password.txt", "r");
    rewind(fp);
    //char *accesMessage = "200 User test granted to access.\n";
    while (1) {
        fscanf(fp, "%[^:]:%s\n", fileName, filePassword);
        if (strcmp(fileName, name) == 0 && strcmp(filePassword, password) == 0) {
           // send(fd, accesMessage, strlen(accesMessage), 0);
            printf("USER OK!");
        }
        if (feof(fp))
            break;
    }
    //printf("USER denied");
    fclose(fp);
}


int main(int argc, char **argv) {
    char *dir;
    char *port;
    char *passFile;
    int login = 0;
    int server, client;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    int length,fd,rcnt,optval;
    pid_t pid;

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
     //LIST(dir);
     //GET(passFile);
     //USER("yusuf","12134"); // bu true retunrlemeli

        if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) {
            perror("Can't create socket!");
            return(1);
        }

        memset( &local_addr, 0, sizeof(local_addr) );
        memset( &remote_addr, 0, sizeof(remote_addr) );



    }








    return 0;
}
