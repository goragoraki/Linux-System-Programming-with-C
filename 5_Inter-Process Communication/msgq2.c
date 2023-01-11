#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include "msg.h"

#define MSG_MODE (0600)
// msgq를 열어 data를 주고 받고 하는 함수
int main(){
	int msqid, n;
	MsgType msg;
	// msgq id를 받음
	if((msqid = msgget(MSG_KEY, MSG_MODE))<0){
		perror("msgget");
		exit(1);
	}
	msg.type=MSG_REQUEST;
	sprintf(msg.data,"This is a request from %d", getpid());
	// msgq에 data 씀
	if(msgsnd(msqid,(char*)&msg, sizeof(msg),0)<0){
		perror("msgsnd");
		exit(1);
	}
	printf("Sent a request ...");
	// msgq에서 data를 읽음
	if((n = msgrcv(msqid, (char*)&msg, sizeof(msg),MSG_REPLY,0))<0){
		perror("msgrcv");
		exit(1);
	}
	printf("Received reply: %s\n",msg.data);
	return 0;
}
