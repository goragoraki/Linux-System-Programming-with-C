#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// ls -l | more을 수행하는 함수
int main(){
	int fd[2];
	pid_t child;
	if(pipe(fd) == -1){ // pipe 생성
		perror("pipe");
		exit(1);
	}
	child = fork(); // fork
	if(child <0){
		perror("fork");
		exit(1);
	}else if(child == 0){ // 자식 프로세스 일 경우
		close(1); // STDOUT_FILENO 를 닫아줌.
		close(fd[0]); 
		if(dup(fd[1]) == -1){ // STDOUT_FILENO에 fd[0]를 연결
			perror("dup");
			exit(1);
		}
		// ls -l 수행
		// 결과 값이 fd[1]로 들어감
		if((execlp("ls", "ls", "-l",NULL)) ==-1){ 
			perror("execlp");
			exit(1);
		}
	}else{
		close(0); // STDIN_FILENO를 닫음
		close(fd[1]);
		if(dup2(fd[0], 0) ==-1){ // STDIN_FILENO에 fd[0]을 연결
			perror("dup");
			exit(1);
		}
		// 자식이 준 출력 값을 이용하여 more 수행
		if(execlp("more", "more", NULL)==-1){
			perror("more");
			exit(1);
		}
	}
	return 0;
}
