
#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */
#include <netdb.h> /*gethostbyname function */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>


#define MAXLINE 1024

void *handle();

int main(int argc, char **argv)
{
    pthread_t thread_ids[1000];
    printf("Before thread start:\n");
    for (int i = 0; i < 1000; ++i)
    {

        int erro = pthread_create(&thread_ids[i], NULL, handle, NULL); 
        if (erro !=0)
        {
            printf("create thread: %d error!\n",thread_ids[i]);
        }
        pthread_join(thread_ids[i],NULL);
       
    }
    printf("After thread End:\n");
    return 0;
}

void *handle()
{
    char * servInetAddr = "127.0.0.1";
    int servPort = 4444;
    char buf[MAXLINE];
    int connfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
    int n;
    //create socket
    connfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(servPort);
    inet_pton(AF_INET, servInetAddr, &servaddr.sin_addr);
    //connect to server
    if (connect(connfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
    }
    printf("welcome to echoclient\n");
    //thread main loop
    for (;;) {
        if (fgets(sendline, MAXLINE, stdin) == NULL) {
            break;//read eof
        }
        /*
        //也可以不用标准库的缓冲流,直接使用系统函数无缓存操作
        if (read(STDIN_FILENO, sendline, MAXLINE) == 0) {
            break;//read eof
        }
        */
        pthread_t thread_id = pthread_self();
        time_t t;
        time(&t);
        n = sprintf (sendline, "this is thread: %d,time is:%f", thread_id,t);
        n = write(connfd, sendline, n);
        n = read(connfd, recvline, MAXLINE);
        if (n == 0) {
            printf("echoclient: server terminated prematurely\n");
            break;
        }
        write(STDOUT_FILENO, recvline, n);
        //如果用标准库的缓存流输出有时会出现问题
        //fputs(recvline, stdout);
        sleep(0.5);
    }
}