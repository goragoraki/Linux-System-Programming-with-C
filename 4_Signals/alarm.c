#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
static unsigned int alsecs; // 알림 울릴 시간 변수
// 알람 신호에 대한 핸들러 함수
void alhandler(int signo){
	// 유닉스에서는 핸들러가 사라지기 때문에 다시 생성해주는 코드
	if(signal(SIGALRM, alhandler) == SIG_ERR){
		perror("signal");
		exit(0);
	}
	alarm(alsecs); // alsecs 후에 알람 울림
	printf("."); 
	fflush(stdout); // stdout 버퍼에 있는것 비워줌
	return ;
}
// 알람 설정 함수
int setalarm(unsigned int nsecs){
	// 알람 신호에 대한 핸들러를 만듬
	if(signal(SIGALRM, alhandler)==SIG_ERR){
		return -1;
	}
	alsecs = nsecs; // 알람 울릴 시간 저장
	alarm(nsecs); // nsecs 후 알람 울림
	return 0;
}

int main(){
	printf("Doing something every one seconds\n");
	setalarm(1); // 1초로 설정
	for( ; ; )
		pause(); // 신호 기다림
	return 0;
}
