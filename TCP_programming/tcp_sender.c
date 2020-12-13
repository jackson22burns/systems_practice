#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
/*				UDP vs TCP
./udp_sender 134.126.141.40 3333 hello	-	We don't know if the packet arrived at the server
./tcp_sender 134.126.141.40 3333 		-	W/ TCP, we have reliable communication. eventually connect will time out (with UDP we don't get this feedback)
	If you try to connect to a port that doesn't have a listening process on it --> during the connection setup, the connection host 
	will notice this and send an internet control message to the sender letting them know
			~TCP lets you know if the packet doesn't arrive (reliable communication~
*/
int main(int argc, char *argv[]) {

	int sockfd;
	struct sockaddr_in address;
	
	if(argc != 3) {
		printf("Usage: %s server_address port_address\n", argv[0]);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&address, 0, sizeof(address)); 		//zero out 
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[2])); 	//atoi converts string to int.  htons() converts values to network byte order (big-endian)
	inet_pton(AF_INET, argv[1], &(address.sin_addr));		//valid IP address
	
	if(connect(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0) { //will block until the connection is established or we receive an error
		printf("Error connecting to host %s %s: %s\n", argv[1], argv[2], strerror(errno));
		exit(1);
	}
	
	while(1) {
		
		char *line = readline("Enter data to be sent: ");
		if (write(sockfd, line, strlen(line)) < 0) {
			printf("Error: %s\n", strerror(errno));
		}
	
		
		
	}
	
	close(sockfd);
}