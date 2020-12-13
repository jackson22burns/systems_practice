#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 255

int main(int argc, char *argv[]) {

	int sockfd;
	struct sockaddr_in address;
	char buffer[BUFSIZE+1];
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&address, 0, sizeof(address)); 		//zero out 
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[1])); 	//atoi converts string to int.  htons() converts values to network byte order (big-endian)
	address.sin_addr.s_addr = INADDR_ANY;		//IP address
	
	if(bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0) { //always check for an error: port number below 1024 are protected. port could be in use
		printf("Error binding to socket: %s\n", strerror(errno));
	}
	
	listen(sockfd, 0);
	
	while(1) {
		
		//int conn_socket = accept(sockfd, NULL, NULL);
		
			
		struct sockaddr_in sender; 				//for address info about who sent it
		socklen_t sender_len = sizeof(sender); 	//must specify the correct size
		char name_buf[INET_ADDRSTRLEN];
		
		printf("Waiting for incoming connection\n");
		int conn_socket = accept(sockfd, (struct sockaddr*)&sender, &sender_len); //performs blocking 
		printf("Server: accepted connection from %s, port %d\n",
				inet_ntop(AF_INET, &(sender.sin_addr), name_buf, INET_ADDRSTRLEN), 
				ntohs(sender.sin_port));
				
		
		
		while(1) { // for the actual communication with the client
			ssize_t bytes_read = read(conn_socket, buffer, BUFSIZE);
			if(bytes_read < 0) {
				printf("Error reading from connection socket: %s\n", strerror(errno));
			}
			if(bytes_read == 0) break; //when connection gets terminated (one of the partys does ctr + C)
			write(STDOUT_FILENO, buffer, bytes_read);
		}
		
		
	}
}