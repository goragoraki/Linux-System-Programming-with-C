#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "udp.h"

int Sockfd;
// error handler
// close socket
void CloseServer(){
	close(Sockfd);
	printf("\n UDP Server exit.....\n");
	exit(0);
}
// udp server
int main(){
	int cliAddrLen,n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;
	
	// make error handler about sigint
	signal(SIGINT, CloseServer);
	
	// make socket
	// IPv4, udp
	if((Sockfd=socket(PF_INET, SOCK_DGRAM,0))<0){
		perror("socket");
		exit(1);
	}
	
	//make server address
	bzero((char*)&servAddr,sizeof(servAddr)); //set 0
	servAddr.sin_family=PF_INET; // IPv4
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);// pick random LAN card among empty
	servAddr.sin_port = htons(SERV_UDP_PORT); // port
	
	// bind
	if(bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr))<0){
		perror("bind");
		exit(1);
	}

	printf("UDP Server started...\n");
	cliAddrLen=sizeof(cliAddr);
	while(1){
		// receive msg from client
		if((n=recvfrom(Sockfd, (char*)&msg,sizeof(msg), 0, (struct sockaddr*)&cliAddr,
						&cliAddrLen))<0){
							perror("recvform");
							exit(1);
						}
	
		printf("Received request: %s....",msg.data);

		msg.type=MSG_REPLY;
		sprintf(msg.data,"This is a reply from %d.", getpid());
		
		//send msg to client
		if(sendto(Sockfd, (char*)&msg,sizeof(msg),0,(struct sockaddr*)&cliAddr,cliAddrLen)<0){
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}

	return 0;
}
