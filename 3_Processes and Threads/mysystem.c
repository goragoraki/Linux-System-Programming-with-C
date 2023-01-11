#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
// system()와 같이 동작하는 함수
void mysystem(char* str){
	char* cmd[512] = {"bash", "-c"}; // execv에 넣을 벡터를 담을 변수	
	int idx = 2;
	cmd[idx] = str; // 벡터에 명령어를 넣어줌
	idx++;
	cmd[idx] = NULL; // 벡터 마지막에 NULLㅡㅇㄹ 넣어줌
	pid_t pid;
	if((pid=fork())<0){ // 새로운 프로세스 생성
		perror("fork");
		exit(1);
	}else if(pid ==0){ //자식 프로세스 일 경우
		// bin/bash -c command 형태로 실행하도록 함.
		if(execv("/bin/bash",cmd)<0){
			perror("execv");
			exit(1);
		}
	}else // 부모 프로세스 일 경우 자식이 끝날때까지 기다림
		waitpid(pid, NULL, 0);
	
}
//mysystem 실행
int main(){
	mysystem("date");
	mysystem("ls -l");

	return 0;
}
