#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#define MAX_BUF 1024 // 버퍼 최대 크기

// 분리된 파일 두 개를 한 파일로 합쳐주는 함수.
int main(int argc, char* argv[]){
	if(argc!=4){
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}
	
	// 읽을 파일 2개 쓸 파일 1개를 저장하는 변수
	FILE* fp1, *fp2,*fp3; 

	// 파일을 읽기모드 바이너리 파일로 열어 파일 번호 저장
	if((fp1 = fopen(argv[1], "rb"))==NULL){
		perror("fopen");
		exit(1);
	}
	// 파일을 읽기모드 바이너리 파일로 열어 파일 번호 저장
	if((fp2 = fopen(argv[2], "rb"))==NULL){
		perror("fopen");
		exit(1);
	}
	// 파일을 쓰기모드 바이너리 파일로 열어 파일 번호 저장
	if((fp3 = fopen(argv[3], "wb"))==NULL){
		perror("fopen");
		exit(1);
	}
	
	char buf[MAX_BUF]; // 읽은 내용을 저장하는 변수
	int count; // 읽은 크기를 저장할 변수

	// 파일을 읽고 읽은 크기 저장
	while((count = fread(buf,1,MAX_BUF,fp1))>0){
		fwrite(buf,1,count,fp3); // 파일을 읽은 만큼 씀.
	}
	// 파일을 읽고 읽은 크기 저장
	while((count = fread(buf,1,MAX_BUF,fp2))>0){
		fwrite(buf,1,count, fp3); // 파일을 읽은 만큼 씀.
	}
	fclose(fp1); //close file
	fclose(fp2);
	fclose(fp3);
	return 0;
}	
