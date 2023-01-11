#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define MAX_BUF 1024 // buf의 최대 값
				
// 바이너리 파일로 열어 다른 파일에 복사함.
int main(int argc, char* argv[]){
	if(argc!=3){
		perror(argv[0]);
		exit(1);
	}
	FILE* fp1; // 복사할 파일
	FILE* fp2; // 복사 받을 파일

	// 파일을 읽기모드 바이너리 파일로 열어 파일번호 저장
	if((fp1 = fopen(argv[1], "rb"))==NULL){
		printf("open1 error");
		exit(1);
	}
	// 파일을 쓰기모드 바이너리 파일로 열어 파일번호 저장
	if((fp2 = fopen(argv[2], "wb"))==NULL){
		printf("open2 error");
		exit(1);
	}
	int cnt; // 읽은 크기를 저장할 변수
	char buf[MAX_BUF]; //읽은 내용을 저장할 변수 

	// 파일 내용을 읽고 buf에 내용을 저장하고 cnt에 읽은 크기를 저장함.
	while((cnt = fread(buf,1, MAX_BUF, fp1))>0){
		fwrite(buf,1 , cnt, fp2); // cnt만큼 버퍼의 내용을 파일에 씀
	}
	fclose(fp1); // close file
	fclose(fp2);
	return 0;
}

