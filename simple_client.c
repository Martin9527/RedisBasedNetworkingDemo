#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "ae.h"
#include "anet.h"

#define PORT 4444
#define MAX_LEN 1024

//存放错误信息的字符串
char g_err_string[1024];

int main()
{

    printf("Start\n");

    int fd = anetTcpConnect(g_err_string,NULL,PORT)
    if (ANET_ERR == fd)
        fprintf(stderr, "Connected port %d error: %s\n", PORT, g_err_string);
    char sendbuf[1024];
    char recvbuf[1024];
    for ( ; ; ) {
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        FD_SET(0, &rset); 
        maxfdp1=sockfd+1;
        if((select(maxfdp1, &rset, NULL, NULL, NULL) )<=0){
            perror("select");
        }else{
            if (FD_ISSET(0,&rset)){
                fgets(sendbuf, MAXLINE, stdin);
                n=send(sockfd,sendbuf,strlen(sendbuf)-1,0);
                if(n>0)
                    printf("send: %s",sendbuf);
                else
                    printf("send: %s error,the erro cause is %s:%s/n",sendbuf,errno,strerror(errno));
                    bzero(sendbuf,strlen(sendbuf));
            }

            if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
                n=recv(sockfd, recvbuf, MAXLINE,0) ;
                if(n<0) {
                    perror("str_cli: server terminated prematurely");
                }else if(n==0)
                {
                    printf("sever shutdown!");
                    exit(-1);
                }
                //recvbuf
                recvbuf[n]='/0';
                printf("receive :%s/n",recvbuf);
                fflush(stdout);
                bzero(recvbuf,strlen(recvbuf));
            }
        }
    }
    printf("End\n");
    close(sockfd);
    return 0;
}

int threadProcedure()
{
    unsigned long i = 0;
    pthread_t id = pthread_self();
    printf("Thread ID: %d,Start\n"%id);
    int sockfd = anetTcpConnect(g_err_string,NULL,PORT)
    if (ANET_ERR == sockfd)
        fprintf(stderr, "Connected port %d error: %s\n", PORT, g_err_string);
    char sendbuf[1024];
    char recvbuf[1024];
    FD_SET rset;
    for (int i = 0; i < 10000; ++i)
    {
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        FD_SET(0, &rset); 
        maxfdp1=sockfd+1;
        if((select(maxfdp1, &rset, NULL, NULL, NULL) )<=0){
            perror("select");
        }else{
            if (FD_ISSET(0,&rset)){
                sendbuf = "Thread ID: %d,Data:%d/0"%(id,i)
                n=send(sockfd,sendbuf,strlen(sendbuf)-1,0);
                if(n>0)
                    printf("send: %s",sendbuf);
                else
                    printf("send: %s error,the erro cause is %s:%s/n",sendbuf,errno,strerror(errno));
                    bzero(sendbuf,strlen(sendbuf));
            }

            if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
                n=recv(sockfd, recvbuf, MAXLINE,0) ;
                if(n<0) {
                    perror("str_cli: server terminated prematurely");
                }else if(n==0)
                {
                    printf("sever shutdown!");
                    exit(-1);
                }
                //recvbuf
                recvbuf[n]='/0';
                printf("receive :%s/n",recvbuf);
                fflush(stdout);
                bzero(recvbuf,strlen(recvbuf));
            }
        }
        sleep(0.5)
    }
    close(sockfd);
    return 0;

}

int anotherExample()
{
    int sock; 
    char send_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr; 

    host = gethostbyname("127.0.0.1");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;    
    server_addr.sin_port = htons(5000);  
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8);

    if (connect(sock, (struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
    {
        perror("Connect");
        exit(1);
    }

    while(1) {
        printf("\nSEND (q or Q to quit) : ");
        gets(send_data);

        if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
            send(sock,send_data,strlen(send_data), 0);
        else
            break;
    }
    close(sock);
    return 0;
}