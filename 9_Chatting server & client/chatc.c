#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "chat.h"

#define	MAX_BUF		256
// chat client with multi thread version

int		Sockfd;
pthread_t tidR, tidS;

// send user ID
void SendID(void){
	char	buf[MAX_BUF];
	int		count, n;

	printf("Enter ID: ");
	fflush(stdout);
	fgets(buf, MAX_BUF, stdin);
	*strchr(buf, '\n') = '\0';
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {
		perror("send");
		exit(1);
	}
	printf("Press ^C to exit\n");
}
// Receive Msg from server
void ReceiveMsg(void* dummy){
	char	buf[MAX_BUF];
	int		count, n;
	
	// set thread option
	// can cancel this thread anyone
	// immediately terminate thread
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		exit(1);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		exit(1);
	}

	while (1)  { 
		// wait server msg and recevie
		if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0)  {
			perror("recv");
			exit(1);
		} 
		if (n == 0)  { // if server terminate
			fprintf(stderr, "Server terminated.....\n");
	
			if(pthread_cancel(tidS)){ // terminate SendMsg thread
				perror("pthread_cancel");
				exit(1);
			}

			if(pthread_join(tidS,NULL)<0){ // wait to terminate SendMsg thread
				perror("pthread_join");
				exit(1);
			}
			close(Sockfd); // close socket
			pthread_exit(NULL);
		}
		printf("%s", buf);
	}
}
//Send msg to server
void SendMsg(void* dummy){
	char	buf[MAX_BUF];
	int		count, n;
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		exit(1);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		exit(1);
	}
	while(1){
		// get client msg 
		fgets(buf, MAX_BUF, stdin);
		
		//send client msg to server
		if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0)  {
			perror("send");
			exit(1);
		}
	}
}
// close client	
void CloseClient(int signo){
	// cancel all thread and close socket
	if (pthread_cancel(tidR))  {
		perror("pthread_cancel");
		exit(1);
	}
	if (pthread_cancel(tidS))  {
		perror("pthread_cancel");
		exit(1);
	}
	if (pthread_join(tidR, NULL)<0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tidS, NULL)<0)  {
		perror("pthread_join");
		exit(1);
	}
	close(Sockfd);
	printf("\nChat client terminated.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	struct sockaddr_in	servAddr;
	struct hostent		*hp;
	
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}
	// make socket
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	// set server address
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  {
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
	//connect
	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	signal(SIGINT, CloseClient);

	SendID(); // Send ID to Server

	// thread1 : Receive Msg
	if (pthread_create(&tidR, NULL, (void *)ReceiveMsg, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	
	// thread2 : Send Msg
	if (pthread_create(&tidS, NULL, (void *)SendMsg, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_join(tidR, NULL)<0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tidS, NULL)<0)  {
		perror("pthread_join");
		exit(1);
	}
	
	return 0;
}
