#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "fifo.h"
//cilent 함수
int main(){
	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	Msgtype msg;
	// 자신 고유의 fifo 이름 생성
	sprintf(fname, ".fifo%d", getpid());
	// fifo 파일 생성
	if(mkfifo(fname,0600)<0){
		perror("mkfifo");
		exit(1);
	}
	// cilent fifo 파일 open
	if((fd=open(fname, O_RDWR))<0){
		perror("open");
		exit(1);
	}
	// 서버 fifo 파일 open
	if((sfd=open(SERV_FIFO,O_RDWR))<0){
		perror("open");
		exit(1);
	}
	// 자신의 fifo 파일 이름을 data에 써줌
	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This request from %d.", getpid());
	// server fifo 파일에 data를 써줌
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request...");
	
	// server 응답을 기다림
	if((n=read(fd, (char *)&msg, sizeof(msg)))<0){
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);
	close(fd);
	close(sfd);
	// 자신의 fifo 파일 삭제
	if(remove(fname)<0){
		perror("remove");
		exit(1);
	}
	return 0;
}
