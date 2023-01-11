#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"

#define MSG_MODE (0600|IPC_CREAT)
// msgq를 만들고 data를 받고 응답하는 함수
int main(){
	int msqid, n;
	MsgType msg;

	// msgq 생성
	if((msqid = msgget(MSG_KEY,MSG_MODE))<0){
		perror("msgget");
		exit(1);
	}
	printf("Waiting Request.....\n");
	// msgq에 data가 들어오면 읽음
	if((n=msgrcv(msqid,(char*)&msg,sizeof(msg),1,0))<0){
		perror("msgrcv");
		exit(1);
	}
	printf("Received request: %s ....", msg.data);

	msg.type = MSG_REPLY; 
	sprintf(msg.data, "This is a reply from %d", getpid());
	// data를 보냄
	if(msgsnd(msqid, (char*)&msg,sizeof(msg),0)<0){
		perror("msgsnd");
		exit(1);
	}
	printf("Replied.\n");
	sleep(1);
	// msgq 삭제
	if(msgctl(msqid,IPC_RMID,0)<0){
		perror("msgctl");
		exit(1);
	}
	printf("complete\n");
	return 0;
}
