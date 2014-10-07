
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

//TODO: fix includes
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_PORT 8080

int main(int argc, char *argv[])
{
        int listenSocket, clientSocket, clientAddrLen;

        char * receivedData;

        struct sockaddr_in myAddr, clientAddr;

        listenSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
                perror("Sever: could not open listener socket.\n");
                exit(1);
        }

        myAddr.sin_family = AF_INET;
        myAddr.sin_port = htons(LISTEN_PORT);

        listen(listenSocket, 3);

        clientAddrLen = sizeof clientAddr;
        while (1)
        {
                clientSocket = accept(listenSocket,
                                      (struct sockaddr *) & clientAddr,
                                      & clientAddrLen);
                if (clientSocket < 0)
                {
                        perror("Sever: could not accept client socket.\n");
                        exit(1);
                }
                
        }

        return 0;
}
