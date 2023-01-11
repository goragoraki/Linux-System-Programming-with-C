#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
// SIGALRM을 처리할 handler function
void sigalarm(int signo){
	return;
}
// 주어진 시간동안 기다리는 함수
unsigned int mysleep(unsigned int nsecs){
	// SIGALRM에 대한 핸들러를 만듬
	if(signal(SIGALRM, sigalarm) == SIG_ERR){
		return nsecs;
	}
	alarm(nsecs); // SIGALRM 신호를 nsecs 후에 보냄
	pause(); // 신호를 기다리고 있음
	return alarm(0);
}
int main(){
	printf("Wait for 5 seconds...\n");
	mysleep(5);
	return 0;
}

