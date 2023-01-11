#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
//파일 내용을 다른 파일에 복사함.
int main(int argc, char* argv[]){
	//파일을 2개 받아야함
	if(argc!=3){
		perror("argv[0]");
		exit(1);
	}
	int fd1, fd2; // 복사할 파일 1개, 복사받을 파일 1개인 변수
	char buf; // 파일을 읽은 것을 임시 저장할 변수
	
	//파일을 읽기 모드로 염
	if((fd1=open(argv[1], O_RDONLY))<0){
		perror("open1");
		exit(1);
	}

	// 파일을 쓰기모드로 여는데 파일이 없으면 생성함.
	// 파일에 내용이 적혀있을 경우 다 지우고 씀
	// rw-r--r-- 권한으로 생성
	if((fd2=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644))<0){
		perror("open2");
		exit(1);
	}
	int cnt; // 읽은 버퍼 사이즈 저장 변수

	// 파일을 읽어 버퍼에 저장
	while((cnt=read(fd1,&buf,1)>0)){
		write(fd2,&buf,cnt); //읽은 파일 버퍼를 fd2파일에 씀.
	}
	close(fd1); //close file
	close(fd2); //close file
	return 0;
}
