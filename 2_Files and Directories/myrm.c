#include <stdio.h>
#include <stdlib.h>
// 파일 혹은 디렉토리를 지우는 함수
int main(int argc, char* argv[]){
	if(argc==1){
		perror(argv[0]);
		exit(1);
	}
	// argv에 담긴 이름의 파일 혹은 디렉토리를 지움
	while(--argc){
		if(remove(argv[argc])<0){
			perror("remove");
			exit(1);
		}
	}
	return 0;
}

