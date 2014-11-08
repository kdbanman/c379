#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>

#define PORT 8080
#define SERVER_ADDR     "127.0.0.1"     /* localhost */
#define MAXBUF          4096

void snd(char * msg)
{
    int sockfd;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    /*---Open socket for streaming---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Socket");
        exit(errno);
    }

    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    if ( inet_aton(SERVER_ADDR, &dest.sin_addr.s_addr) == 0 )
    {
        perror(SERVER_ADDR);
        exit(errno);
    }

    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
        perror("Connect ");
        exit(errno);
    }

    bzero(buffer, MAXBUF);
    printf("Sending:\n%s", msg);
    write(sockfd, msg, strlen(msg));
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Received:\n");
    printf("%s\n", buffer);

    close(sockfd);


}

int main()
{
    snd("GET /.vimrc\n\n");
    snd("GET /rekt\n\n");
    snd("GET /nope\n\n");

    return 0;
}


