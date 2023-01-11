#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256
// chat server with select system calls

//client socket info
typedef	struct  {
	int			sockfd; 
	int			inUse; 
	pthread_t	tid;
	char		uid[MAX_ID];
	int			First; // for receive User ID
}
	ClientType;

int				Sockfd;
fd_set reads, cpy_reads;
ClientType		Client[MAX_CLIENT];

// return available Client ID
int GetID(){
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (! Client[i].inUse)  {
			Client[i].inUse = 1;
			return i;
		}
	}
}
//send msg to other client
void SendToOtherClients(int id, char *buf){
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif

	for (i = 0 ; i < MAX_CLIENT ; i++)  { 
		if (Client[i].inUse && (i != id))  { // give unused ID to client
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
				perror("send");
				exit(1);
			}
		}
	}
}
// process client socket
void ProcessClient(int id) {
	char	buf[MAX_BUF];
	int		n;
	
	// first msg is client ID
	if(Client[id].First==0){
		if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  {
			perror("recv");
			exit(1);
		}
		Client[id].First=1;
		printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
	}else{ // client msg
		if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  {
			perror("recv");
			exit(1);
		}
		if (n == 0)  { // log out client
			printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);
			
			FD_CLR(Client[id].sockfd, &reads); // select descriptor 정보에서 삭제 시킴
			close(Client[id].sockfd); // close socket
			Client[id].inUse = 0; // delete ID

			strcpy(buf, "log-out.....\n");
		}

		SendToOtherClients(id, buf);
	}
}
// socket close 
void CloseServer(int signo){
	int		i;
	close(Sockfd);
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			close(Client[i].sockfd);
		}
	}
	printf("\nChat server terminated.....\n");

	exit(0);
}
// chat server with select system call
int main(int argc, char *argv[]) {
	int					newSockfd, cliAddrLen, id, one = 1;
	struct sockaddr_in	cliAddr, servAddr;
	int fd_max, fd_num, i;
	int sockIdx[256];

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
		exit(1);
	}
	// server id 설정
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	//bind
	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);
	printf("Chat server started.....\n");

	FD_ZERO(&reads);
	FD_SET(Sockfd,&reads); // tcp sock bit on
	fd_max=Sockfd; 

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		cpy_reads=reads;

		// descriptor 받음
		if((fd_num=select(fd_max+1,&cpy_reads,(fd_set*)NULL,(fd_set*)NULL,(struct timeval*)NULL))<0){
			perror("select");
			exit(1);
		}
		if(fd_num==0)
			continue;
		for(i=0;i<fd_max+1;i++){ // 바뀐 descriptor 검사
			if(FD_ISSET(i,&cpy_reads)){ // socket i가 변동 사항이 있으면
				if(i==Sockfd){ // 서버 일 경우 client의 연결 요청
					newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
					if (newSockfd < 0)  {
						perror("accept");
						exit(1);
					}
					FD_SET(newSockfd,&reads); // 새로운 client descriptor 추가
					if(fd_max<newSockfd)
						fd_max=newSockfd; 
					
					id = GetID(); // ID 부여
					sockIdx[newSockfd]=id; // ID index 저장
					Client[id].sockfd = newSockfd; // client socket
					Client[id].First=0; // User ID 전달 받았는지 여부
				}
				else{ // 변동된 descriptor가 client 소켓일 경우
					ProcessClient(sockIdx[i]);  // client 처리 함수
				}
			}
		}
	}
	return 0;
}
