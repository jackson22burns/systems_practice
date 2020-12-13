#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 10

int main(int argc, char *argv[]) {

	int sockfd;
	struct sockaddr_in address;
	char buffer[BUFSIZE+1];
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&address, 0, sizeof(address)); 		//zero out 
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[1])); 	//atoi converts string to int.  htons() converts values to network byte order (big-endian)
	address.sin_addr.s_addr = INADDR_ANY;		//IP address
	
	if(bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0) { //always check for an error: port number below 1024 are protected. port could be in use
		printf("Error binding to socket: %s\n", strerror(errno));
	}
	
	while(1) {
		//ssize_t bytes_received = recvfrom(sockfd, buffer, BUFSIZE, 0, NULL, NULL);
		//write(STDOUT_FILENO, buffer, bytes_received);
		
		//Who sent us the data??
		struct sockaddr_in sender; 				//for address info about who sent it
		socklen_t sender_len = sizeof(sender); 	//must specify the correct size
		char name_buf[INET_ADDRSTRLEN];
		
		ssize_t bytes_received = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr*)&sender, &sender_len);
		buffer[bytes_received] = '\0';
		printf("Received data from %s,  port %d: %s\n",
				inet_ntop(AF_INET, &(sender.sin_addr), name_buf, INET_ADDRSTRLEN), 
				ntohs(sender.sin_port), buffer);
		
		
	}
}