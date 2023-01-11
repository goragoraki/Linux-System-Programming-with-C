#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"
// error handler function
void sigintHandler(int signo){
	// 서버 종료시 서버 fifo 파일 삭제
	if(remove(SERV_FIFO)<0){
		perror("remove");
		exit(1);
	}
	exit(0);
}
// 서버 함수
int main(){
	int fd, cfd, n;
	Msgtype msg;
	// SIGINT  error handler 함수 생성
	if(signal(SIGINT, sigintHandler)==SIG_ERR){
		perror("signal");
		exit(1);
	}
	
	// server fifo 파일 생성
	if(mkfifo(SERV_FIFO,0600)<0){
		if(errno!=EEXIST){
			perror("mkfifo");
			exit(1);
		}
	}
	// fifo 파일 open
	if((fd=open(SERV_FIFO,O_RDWR))<0){
		perror("open");
		exit(1);
	}
	while(1){
		printf("Waiting client ......\n");	
		// msg에 값이 들어올때까지 기다림
		// 들어오면 값을 읽음
		if((n=read(fd,(char *)&msg, sizeof(msg)))<0){
			//interrupt가 들어 왔을 때는 에러가 아니므로
			//다시 값이 들어올 때까지 기다림
			if(errno == EINTR)
				continue;
			else{
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s...", msg.data);
		// cilent에 쓸 파일 open
		// msg에 담긴 cilent fifo 이름을 이용
		if((cfd=open(msg.returnFifo, O_WRONLY))<0){
			perror("open");
			exit(1);
		}
		// data에 값을 써주고
		sprintf(msg.data, "This is a reply from %d...", getpid());
		// cilent fifo 파일에 data를 씀
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd);
		printf("Replied.\n");		
	}

	return 0;
}
