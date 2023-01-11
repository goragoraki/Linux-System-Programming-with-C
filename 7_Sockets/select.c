#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "select.h"

int TcpSockfd;
int UdpSockfd;

// error handler
// close Tcp, Udp socket
void CloseServer(){
	close(TcpSockfd);
	close(UdpSockfd);
	printf("\nServer daemon exit......\n");
	exit(0);
}

// make tcp socket
void MakeTcpSocket(){
	struct sockaddr_in servAddr;
	
	// make tcp socket
	// IPv4, tcp
	if((TcpSockfd=socket(PF_INET,SOCK_STREAM,0))<0) {
		perror("bind");
		exit(1);
	}
	
	//set server address
	bzero((char*)&servAddr, sizeof(servAddr)); // set 0
	servAddr.sin_family=PF_INET; // IPv4
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);// set random LAN card among empty
	servAddr.sin_port=htons(SERV_TCP_PORT); // port

	// bind
	if(bind(TcpSockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))<0){
		perror("bind");
		exit(1);
	}

	// listen
	listen(TcpSockfd,5);
}

// process rcp request
void ProcessTcpRequest(){
	int newSockfd, cliAddrLen,n;
	struct sockaddr_in cliAddr;
	MsgType msg;
	
	cliAddrLen=sizeof(cliAddr);
	// accept tcp socket
	newSockfd=accept(TcpSockfd,(struct sockaddr*)&cliAddr,&cliAddrLen);
	if(newSockfd<0){
		perror("accept");
		exit(1);
	}
	//read from cilent
	if((n=read(newSockfd,(char*)&msg,sizeof(msg)))<0){
		perror("read");
		exit(1);
	}
	printf("Received TCP request:%s........",msg.data);
	sprintf(msg.data,"This is a reply from %d",getpid());

	// write to client
	if(write(newSockfd,(char*)&msg,sizeof(msg))<0){
		perror("write");
		exit(1);
	}
	printf("Replied.\n");
	close(newSockfd);
}

// make Udp socket
void MakeUdpSocket(){
	struct sockaddr_in servAddr;
	//make udp socket
	// IPv4, udp
	if((UdpSockfd=socket(PF_INET,SOCK_DGRAM,0))<0){
		perror("socket");
		exit(1);
	}

	//set server address
	bzero((char*)&servAddr,sizeof(servAddr)); // set 0
	servAddr.sin_family = PF_INET; // IPv4 
	servAddr.sin_addr.s_addr= htonl(INADDR_ANY);// random LAN card among empty
	servAddr.sin_port=htons(SERV_UDP_PORT); // port
	
	//bind
	if(bind(UdpSockfd,(struct sockaddr*)&servAddr,sizeof(servAddr))<0){
		perror("bind");
		exit(1);
	}
}

// process udp request
void ProcessUdpRequest(){
	int cliAddrLen,n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen=sizeof(cliAddr);
	// receive client msg
	if((n=recvfrom(UdpSockfd,(char*)&msg,sizeof(msg),0,(struct sockaddr*)&cliAddr,&cliAddrLen))<0){
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request:%s ....",msg.data);

	msg.type=MSG_REPLY;
	sprintf(msg.data,"This is a reply from %d",getpid());

	// send msg to client
	if(sendto(UdpSockfd,(char*)&msg,sizeof(msg),
			0,(struct sockaddr*)&cliAddr,cliAddrLen)<0){
		perror("sendto");
		exit(1);
	}
	printf("Replied\n");
}
// server can process both tcp and udp
int main(){
	fd_set fdvar;
	int count;
	
	//make error handler
	signal(SIGINT,CloseServer);
	
	MakeTcpSocket(); // make tcp socket
	MakeUdpSocket(); // make udp socket

	printf("Server daemon started....\n");

	while(1){
		// tcp, udp bit on
		FD_ZERO(&fdvar); // set 0
		FD_SET(TcpSockfd, &fdvar); // tcp bit on
		FD_SET(UdpSockfd, &fdvar); // udp bit on
		
		// receive descriptors
		if((count=select(10,&fdvar,(fd_set*)NULL,(fd_set*)NULL,(struct timeval*)NULL))<0){
			perror("select");
			exit(1);
		}
		while(count--){
			if(FD_ISSET(TcpSockfd,&fdvar)){ // if tcp description
				ProcessTcpRequest(); 
			}else if(FD_ISSET(UdpSockfd,&fdvar)){ // if udp description
				ProcessUdpRequest();
			}
		}
	}
	return 0;
}
