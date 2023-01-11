#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"
int Sockfd;
// error handler function
// close socket
void CloseServer(){
	close(Sockfd);
	printf("\nTCP Server exit.....\n");
}
// tcp server with fork
// Parent : keep making accept
// Child : process client request
int main(){
	int newSockfd, cliAddrLen,n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	pid_t pid;
	signal(SIGINT, CloseServer);
	
	// make socket
	// IPv4, tcp
	if((Sockfd = socket(PF_INET,SOCK_STREAM,0))<0){
		perror("socket");
		exit(1);
	}
	// set server address
	bzero((char*)&servAddr,sizeof(servAddr)); // set 0
	servAddr.sin_family = PF_INET; // IPv4
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//random LAN card among empty
	servAddr.sin_port = htons(SERV_TCP_PORT); // port
	
	// bind
	if(bind(Sockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))<0){
		perror("bind");
		exit(1);
	}
	//listen
	listen(Sockfd,5);

	printf("TCP Server started....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		// accept
		newSockfd=accept(Sockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
		if(newSockfd<0){
			perror("accept");
			exit(1);
		}
		if((pid=fork())<0){
			perror("fork");
			exit(1);
		}else if(pid==0){ // child process
			// read msg from client
			if((n=read(newSockfd, (char*)&msg, sizeof(msg)))<0){
				perror("read");
				exit(1);
			}
			printf("Received request: %s.....",msg.data);
			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());

			// write msg to client
			if(write(newSockfd,(char*)&msg, sizeof(msg))<0){
				perror("write");
				exit(1);
			}
			printf("Replied.\n");
			usleep(10000);
			close(newSockfd); // close socket
			exit(1);
		}else{ // parent
			// close socket and ready to go next accept
			close(newSockfd);
		}
	}
	return 0;
}
