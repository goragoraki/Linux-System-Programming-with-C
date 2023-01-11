#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tcp.h"
// tcp client
int main(int argc, char *argv[]){
	int sockfd,n;
	struct sockaddr_in servAddr;
	MsgType msg;
	
	// make socket
	// ipv4, tcp
	if((sockfd=socket(PF_INET, SOCK_STREAM,0))<0){
		perror("socket");
		exit(1);
	}

	// make server address
	bzero((char*)&servAddr,sizeof(servAddr)); // set 0
	servAddr.sin_family = PF_INET; // ipv4
	servAddr.sin_addr.s_addr=inet_addr(SERV_HOST_ADDR); // server address
	servAddr.sin_port=htons(SERV_TCP_PORT); // tcp port 
	
	// connect
	if(connect(sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))<0){
		perror("connect");
		exit(1);
	}
	
	msg.type=MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d", getpid());
	
	// write to server
	if(write(sockfd, (char*)&msg, sizeof(msg))<0){
		perror("write");
		exit(1);
	}
	printf("Sent a request....");
	
	//read from server
	if((n=read(sockfd, (char*)&msg, sizeof(msg)))<0){
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd); // close socket
	return 0;
}
