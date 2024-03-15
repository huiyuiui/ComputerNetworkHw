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

int handleClient(int clientSocketFD);

int main()
{
	// Variable define
    struct sockaddr_in serverAddr, clientAddr;
    int serverSocketFD, clientSocketFD;
    socklen_t serverAddrLen = sizeof(serverAddr);
    socklen_t clientAddrLen = sizeof(clientAddr);

	// Setup the server address
    bzero((char*)&serverAddr, serverAddrLen);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Create server socket
	// Error return code 1: creating socket
    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocketFD == -1){
        fprintf(stderr, "Error in creating socket: %s\n", strerror(errno));
        return 1;
    }
    printf("Create server socket success\n");

    // Bind socket
	// Error return code 2: binding socket
    if(bind(serverSocketFD, (struct sockaddr *)&serverAddr, serverAddrLen) == -1){
		fprintf(stderr, "Error in binding socket: %s\n", strerror(errno));
        close(serverSocketFD);
        return 2;
	}
	printf("Binding success\n");

	// Listen request of connetction 
	// Error return code 3: listning request of connetction
	if(listen(serverSocketFD, 10) == -1){
		fprintf(stderr, "Error in listning request of connetction: %s\n", strerror(errno));
		close(serverSocketFD);
		return 3;
	}
	printf("Listening success\n");

	// Handle client requirement
    while (1)
    {
		printf("Waiting for client connection...\n");
		// Accept client request
		// Error return code 4: accepting client request of connetction
        clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if(clientSocketFD == -1){
			fprintf(stderr, "Error in accepting client request of connetction: %s\n", strerror(errno));
			close(serverSocketFD);
			return 4;
		}
		printf("Accept client connection\n");

		// Handle request procedure
        // Error return code 5~6: handling client requirements
        int status = handleClient(clientSocketFD);
		if(status != 0){
			fprintf(stderr, "Error in handling client requirements: %s\n", strerror(errno));
			close(serverSocketFD);
			close(clientSocketFD);
			return status;
		}

        close(clientSocketFD);
    }
    return 0;
}


// Handle request function
int handleClient(int clientSocketFD){
    printf("Processing client request\n\n");
    int readBytes, writeBytes;

    while(1){
		// Send message "What's your requirement? 1.DNS 2.QUERY 3.QUIT : " to client
		bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
        strcpy(writeBuffer, "What's your requirement? 1.DNS 2.QUERY 3.QUIT : ");
		writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
		printf("Send message: \"%s\" to client\n", writeBuffer);

		// Get requirement from client
		bzero(readBuffer, LONG_MAX); // Clean up buffer before read
        readBytes = recv(clientSocketFD, readBuffer, LONG_MAX, 0);
        printf("From client requirement number: %s\n", readBuffer);

        // Service according to requirement
		// DNS service
		if(strcmp(readBuffer, "1") == 0){
			// Send message "Input URL address : " to client
			bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
			strcpy(writeBuffer, "Input URL address : ");
			writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
            printf("Send message: \"%s\" to client\n", writeBuffer);

            // Get URL address from client
			bzero(readBuffer, LONG_MAX); // Clean up buffer before read
        	readBytes = recv(clientSocketFD, readBuffer, LONG_MAX, 0);
			printf("URL address from client: \"%s\"\n", readBuffer);
			
			// Get host from given URL address
            struct hostent *host;
            host = gethostbyname(readBuffer);

			bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
			// Error handling: DNS not found
			if(host == NULL){
                strcpy(writeBuffer, "No Such DNS");
            }
			// Response host IP address to client
			else{
				struct in_addr *hostIP;
                hostIP = (struct in_addr*)host->h_addr; // Get IP address
                strcpy(writeBuffer, "address get from domain name : ");
                strcat(writeBuffer, inet_ntoa(*hostIP));
            }
			strcat(writeBuffer, " \n\n");
			fflush(stdin);
            fflush(stdout);
			writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
			printf("Response: %s", writeBuffer);
        }
		// QUERY service
		else if(strcmp(readBuffer, "2") == 0){
			// Send message "Input student ID : " to client
			bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
			strcpy(writeBuffer, "Input student ID : ");
			writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
			printf("Send message: \"%s\" to client\n", writeBuffer);

			// Get student ID from client
			bzero(readBuffer, LONG_MAX); // Clean up buffer before read
        	readBytes = recv(clientSocketFD, readBuffer, LONG_MAX, 0);
			printf("Student ID from client: \"%s\"\n", readBuffer);

			// Open query.txt and handling error
			// Error return code 5: opening file
			FILE* query;
            query = fopen("query.txt", "r");
            if(query == NULL){
				fprintf(stderr, "Error in opening file: %s\n", strerror(errno));
                return 5;
            }

			// Get email from given student ID
            int found = 0;
            int ID = 0;
            int givenID = atoi(readBuffer);
            char Email[256];
			bzero(writeBuffer, LONG_MAX); // Clean up buffer before write
			while(!feof(query)){
                bzero(Email, 256);
                fscanf(query, "%d %s", &ID, Email);
				// Compare given student ID with id in query.txt
				if(ID == givenID){
                    strcpy(writeBuffer, "Email get from server : ");
                    strcat(writeBuffer, Email);
					strcat(writeBuffer, " \n\n");
					fflush(stdin);
					fflush(stdout);		
					writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
                    found = 1;
                    break;
                }
			}

			// Given student ID not found
			if(found == 0){
				strcpy(writeBuffer, "No such student ID");
				strcat(writeBuffer, " \n\n");
				fflush(stdin);
            	fflush(stdout);
				writeBytes = send(clientSocketFD, writeBuffer, LONG_MAX, 0);
			}

			printf("Response: %s", writeBuffer);

			// Close file
			// Error return code 6: closing file
            int status = fclose(query);
			if(status == -1){
				fprintf(stderr, "Error in closing file: %s\n", strerror(errno));
                return 6;
			} 
        }
		// QUIT system
		else if(strcmp(readBuffer, "3") == 0){
			printf("Client connection end. Quit system\n");
            break;
        }
    }

    return 0;
}
