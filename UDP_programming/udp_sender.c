#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 10
/*		./udp_sender 127.0.0.1 1028 hello				*/
int main(int argc, char *argv[]) {

	int sockfd;
	struct sockaddr_in address;
	
	if(argc != 4) {
		printf("Usage: %s server_address server_port data\n", argv[0]);
		exit(0);
	}
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&address, 0, sizeof(address)); 				//zero out 
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[2])); 			//atoi converts string to int.  htons() converts values to network byte order (big-endian)
	inet_pton(AF_INET, argv[1], &(address.sin_addr));	//specify a concrete / valid IP address
	
	sendto(sockfd, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
	
	close(sockfd);
}