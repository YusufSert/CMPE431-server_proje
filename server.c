/* C header files */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include<stdbool.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_BUFLEN 512



void LIST(char *dir,int fd)
{
    DIR *directory = opendir(dir);
    struct dirent *sd;
    while ((sd = readdir(directory)) != NULL)
    {
        char buf[512];
        snprintf(buf, 512, "%s %i\n", sd->d_name, sd->d_reclen);
        send(fd, buf, strlen(buf), 0);
    }


}


void GET(char filename[20], int fd)
{
    char buffer[512];
    FILE *fp = fopen(filename, "r");
    while (fgets(buffer, sizeof(buffer), fp ))
    {
        send(fd, buffer, strlen(buffer), 0);
    }
    fclose(fp);
}


void USER(char name[20], char password[20],char file[20], int fd)
{
    char fileName[20], filePassword[20];
    FILE *fp;
    fp = fopen(file, "r");
    rewind(fp);
    char *accesMessage = "200 User test granted to access.\n";
    while (1) {
        fscanf(fp, "%[^:]:%s\n", fileName, filePassword);
        if (strcmp(fileName, name) == 0 && strcmp(filePassword, password) == 0) {
            send(fd, accesMessage, strlen(accesMessage), 0);
            printf("USER OK!");
        }
        if (feof(fp))
            break;
    }
    //printf("USER denied");
    fclose(fp);
}

void DEL(char filename[20], int fd)
{
    if(remove(filename) == 0)
    {
        char buf[100];
        snprintf(buf,100,"File %s deleted.",filename);
        //send(fd, buf, sizeof(buf), 0);
        printf("%s", buf);
    }
}



////////////////////////////////////////////////////////////////////7

void do_job(int fd,char *dir, char *passFile ) {
    int length,rcnt;
    char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
    int  recvbuflen = DEFAULT_BUFLEN;
    char key1[20];
    char key2[20];
    char key3[20];

    // Receive until the peer shuts down the connection
    do {
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        if (rcnt > 0) {

            sscanf(recvbuf, "%s %s %s", key1, key2, key3);


            if(strncmp("SS", key1, 4) == 0)
            {
                char fileName[20], filePassword[20];
                FILE *fp;
                fp = fopen("text.txt", "r");
                rewind(fp);
                char *accesMessage = "200 User test granted to access.\n";
                while (1) {
                    fscanf(fp, "%[^:]:%s\n", fileName, filePassword);
                    if (strcmp(fileName, key2) == 0) {
                        send(fd, accesMessage, strlen(accesMessage), 0);
                        printf("USER OK!");
                    }
                    if (feof(fp))
                        break;
                }
                //printf("USER denied");
                fclose(fp);
            }

            /// **************** LIST *******************  ///
            if (strncmp("LIST", key1, 4) == 0) {
                LIST(dir,fd);
            }

            ///******************** GET **************************** ///
            if (strncmp("GET", key1, 3) == 0){
                GET(key2,fd);
            }

            if (strncmp("DEL", key1, 3) == 0)
            {
                printf("ss");
                if (remove(key2) == 0) {
                    printf("The file is deleted successfully.");
                } else {
                    printf("The file is not deleted.");
                }
            }
            if (strncmp("QUIT", key1, 4) == 0) {
                rcnt = 0;
                send(fd,"Good bye!",100,0);
            }

            // Echo the buffer back to the sender
           /* rcnt = send( fd, recvbuf, rcnt, 0 );
            if (rcnt < 0) {
                printf("Send failed:\n");
                close(fd);
                break;
            */
            }
            //printf("Bytes sent: %d\n", rcnt);


        /*else if (rcnt == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(fd);
            break;
        }
         */

    } while (rcnt > 0);
}

/////////////////////////////////////////////////////////////////7


int main(int argc, char **argv) {
    char *dir;
    in_port_t servPort;
    char *passFile;
    int login = 0;
    int server, client;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    int length,fd,rcnt,optval;
    pid_t pid;
    int a;

    while ( (a = getopt(argc,argv, "d:p:u")) != -1)
    {
        switch(a)
        {
            case 'd':
                dir = optarg;
                break;
            case 'p':
                servPort = atoi(optarg);
                break;
            case 'u':
                passFile = optarg;
                break;
            case '?':
                if(optopt == 'p' || optopt == 'u' || optopt == 'd')
                    printf( "Option -%c needs argument\n", optopt);
                else
                    printf( "Unknown option -%c. \n", optopt);
                break;
            default:
                printf( "getopt");
        }
    }

    // if(argc == 7){

    //     for(int i=0; i < argc; i++)
    //     {
    //         if(strcmp("-d",argv[i]) == 0){
    //             dir = argv[i+1];
    //             printf("%s\n", dir);
    //         }
    //         if(strcmp("-p",argv[i]) == 0){
    //             servPort = atoi(argv[i+i]);
    //             printf("%d\n",servPort);
    //         }
    //         if(strcmp("-u",argv[i]) == 0){
    //             passFile = argv[i+1];
    //             printf("%s\n",passFile);
    //         }
    //     }
    //         login = 1;
    // }
    // else
    // {
    //     printf("Give correct arguments");
    // }


/* Open socket descriptor */
    if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Can't create socket!");
        return(1);
    }


/* Fill local and remote address structure with zero */
    memset( &local_addr, 0, sizeof(local_addr) );
    memset( &remote_addr, 0, sizeof(remote_addr) );

/* Set values to local_addr structure */
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(servPort);

// set SO_REUSEADDR on a socket to true (1):
    optval = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
    {
        /* could not start server */
        perror("Bind error");
        return(1);
    }

    if ( listen( server, SOMAXCONN ) < 0 ) {
        perror("listen");
        exit(1);
    }

    printf("Concurrent  socket server now starting on port %d\n", servPort);
    printf("Wait for connection\n");

    while(1) {  // main accept() loop
        length = sizeof remote_addr;
        if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
            perror("Accept Problem!");
            continue;
        }

        printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));

        /* If fork create Child, take control over child and close on server side */
        if ((pid=fork()) == 0) {
            close(server);
            do_job(fd, dir, passFile);
            printf("Child finished their job!\n");
            close(fd);
            exit(0);
        }

    }

// Final Cleanup
    close(server);


    return 0;

}
