#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "udp.h"
// udp client
int main(){
	int sockfd,n,peerAddrLen;
	struct sockaddr_in servAddr, peerAddr;
	MsgType msg;
	// make socket
	// IPv4, udp
	if((sockfd=socket(PF_INET,SOCK_DGRAM,0))<0){
		perror("socket");
		exit(1);
	}
	// make server address
	bzero((char*)&servAddr,sizeof(servAddr)); // set 0
	servAddr.sin_family=PF_INET; // IPv4
	servAddr.sin_addr.s_addr=inet_addr(SERV_HOST_ADDR); // set server address
	servAddr.sin_port=htons(SERV_UDP_PORT); // port

	msg.type=MSG_REQUEST;
	sprintf(msg.data,"This is a request from %d", getpid());
	// send msg to server 
	if(sendto(sockfd,(char*)&msg,sizeof(msg),0,(struct sockaddr*)&servAddr,sizeof(servAddr))<0){
		perror("sendto");
		exit(1);
	}
	printf("Sent a request....");
	peerAddrLen=sizeof(peerAddr);

	// receive msg from server
	if((n=recvfrom(sockfd,(char*)&msg,sizeof(msg),0,(struct sockaddr*)&peerAddr,&peerAddrLen))<0){
		perror("recvfrom");
		exit(1);
	}

	printf("Received reply: %s\n",msg.data);
	close(sockfd); // close socket

	return 0;
}
