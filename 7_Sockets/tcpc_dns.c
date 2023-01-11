#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"
// tcp cilent domain address version
int main(int argc, char* argv[]){
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;
	struct hostent *hp;

	// make socket
	// IPv4, tcp
	if((sockfd = socket(PF_INET,SOCK_STREAM,0))<0){
		perror("socket");
		exit(1);
	}
	
	// set server address
	bzero((char*)&servAddr, sizeof(servAddr)); // set 0
	servAddr.sin_family = PF_INET; // IPv4 
	servAddr.sin_port =htons(SERV_TCP_PORT); // port
	if(isdigit(argv[1][0])){ // if address dottd decimal
		servAddr.sin_addr.s_addr=inet_addr(argv[1]);
	}else{ // if domain
		// change 32-bit address for dotted decimal notation
		if((hp=gethostbyname(argv[1]))==NULL){
			perror("hethostbyname");
			exit(1);
		}
		// set server address
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
	// connect
	if(connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr))<0){
		perror("connect");
		exit(1);
	}

	msg.type=MSG_REQUEST;
	sprintf(msg.data,"This is a request from %d", getpid());
	// write msg to server
	if(write(sockfd,(char*)&msg, sizeof(msg))<0){
		perror("write");
		exit(1);
	}
	printf("Sent a request.......");
	
	// read msg from server
	if((n=read(sockfd,(char*)&msg, sizeof(msg)))<0){
			perror("read");
			exit(1);
	}
	printf("Received reply: %s\n", msg.data);
	close(sockfd);

	return  0;
}
