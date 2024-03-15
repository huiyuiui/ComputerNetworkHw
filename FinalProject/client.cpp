#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define LONG_MAX 512

char readBuffer[LONG_MAX];
char writeBuffer[LONG_MAX];

int serverRequest(int serverSocketFD);

int main()
{
    // Variable define
    struct sockaddr_in serverAddr, clientAddr;
    int serverSocketFD;
    socklen_t serverAddrLen = sizeof(serverAddr);
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    // Setup the server address
    bzero(&serverAddr, serverAddrLen);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Create server socket
    // Error return code 1: creating socket
    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocketFD == -1){
        fprintf(stderr, "Error in creating socket: %s\n", strerror(errno));
        return 1;
    }


    // Connect to the server
    // Error return code 11: connecting to server
    if(connect(serverSocketFD, (struct sockaddr *)&serverAddr, serverAddrLen) == -1){
        fprintf(stderr, "Error in connecting to server: %s\n", strerror(errno));
        close(serverSocketFD);
        return 11;
    }

    // Request for service
    // Error return code 12: requesting for server service
    int status = serverRequest(serverSocketFD); // Call procedure you wish to perform
    if(status != 0){
        fprintf(stderr, "Error in requesting for server service: %s\n", strerror(errno));
        close(serverSocketFD);
        return status;
    }

    close(serverSocketFD);
    return 0;
}


// Server request function
int serverRequest(int serverSocketFD){
    int readBytes, writeBytes;

    while (1)
    {
        // Receive message from server: What's your requirement? 1.DNS 2.QUERY 3.QUIT : 
        bzero(readBuffer, LONG_MAX); // Clean up buffer before read
        readBytes = recv(serverSocketFD, readBuffer, LONG_MAX, 0);
        printf("%s", readBuffer);

        // Response requirement to server, shoulde be 1, 2 or 3, else repeat
        while (1)
        {
            bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
            scanf("%s", writeBuffer);
            fflush(stdin);
            fflush(stdout);
            if(strcmp(writeBuffer, "1") == 0 || strcmp(writeBuffer, "2") == 0 || strcmp(writeBuffer, "3") == 0){
                break;
            }
            // Repeat asking
            else{
                printf("Requirement does not exist. Please try again.\n\n");
                printf("What's your requirement? 1.DNS 2.QUERY 3.QUIT : ");
                continue;
            }
        }

        // Request for DNS
        if(strcmp(writeBuffer, "1") == 0){
            writeBytes = send(serverSocketFD, writeBuffer, LONG_MAX, 0);

            // Receive message from server: Input URL address :
            bzero(readBuffer, LONG_MAX); // Clean up buffer before read
            readBytes = recv(serverSocketFD, readBuffer, LONG_MAX, 0);
            printf("%s", readBuffer);

            // Response URL address to server:
            bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
            scanf("%s", writeBuffer);
            writeBytes = send(serverSocketFD, writeBuffer, LONG_MAX, 0);

            // Get IP address from server 
            bzero(readBuffer, LONG_MAX); // Clean up buffer before read
            readBytes = recv(serverSocketFD, readBuffer, LONG_MAX, 0);
            printf("%s", readBuffer);
            fflush(stdin);
            fflush(stdout);
        }
        // Request for QUERY
        else if(strcmp(writeBuffer, "2") == 0){
            writeBytes = send(serverSocketFD, writeBuffer, LONG_MAX, 0);

            // Receive message from server: Input student ID :
            bzero(readBuffer, LONG_MAX); // Clean up buffer before read
            readBytes = recv(serverSocketFD, readBuffer, LONG_MAX, 0);
            printf("%s", readBuffer);

            // Response student ID to server:
            bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
            scanf("%s", writeBuffer);
            writeBytes = send(serverSocketFD, writeBuffer, LONG_MAX, 0);

            // Get Email from server
            bzero(readBuffer, LONG_MAX); // Clean up buffer before read
            readBytes = recv(serverSocketFD, readBuffer, LONG_MAX, 0);
            printf("%s", readBuffer);
            fflush(stdin);
            fflush(stdout);
        }
        // QUIT system
        else if(strcmp(writeBuffer, "3") == 0){
            writeBytes = send(serverSocketFD, writeBuffer, LONG_MAX, 0);
            fflush(stdin);
            fflush(stdout);
            break;
        }

    }

    return 0;
}
