#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define MAX_BUF 1024 // 버퍼 최대 크기

// 파일의 크기를 알려주는 함수
long filesize(FILE* fp){
	//cur : 현재 파일 포인터 위치를 저장하는 변수
	//size : 파일 크기를 저장할 변수
	long cur, size;
	cur = ftell(fp); // 현재 파일포인터 위치 저장
	fseek(fp,0L,SEEK_END); // 파일 포인터를 파일의 끝으로 이동
	size = ftell(fp); // 파일 포인터의 위치를 가져옴
	fseek(fp, cur, SEEK_SET); // 파일 포인터를 초기 위치로 돌려줌
	return size; // 파일 크기 리턴
}
// 파일을 반으로 나누어 쓰는 함수.
int main(int argc, char* argv[]){
	if(argc!=4){
		fprintf(stderr, "Usage: %s source dst1 dst2\n", argv[0]);
		exit(1);
	}
	
	FILE* fp1; // 복사할 파일
	FILE* fp2; // 1번째 파트를 저장할 파일
	FILE* fp3; // 2번째 파트를 저장할 파일
	
	// 파일을 읽기모드 바이너리 파일로 저장 후 파일번호 저장
	if((fp1 = fopen(argv[1], "rb"))==NULL){
		perror("fopen1");
		exit(1);
	}
	// 파일을 쓰기모드 바이너리 파일로 저장 후 파일번호 저장
	if((fp2 = fopen(argv[2], "wb"))==NULL){
		perror("fopen2");
		exit(1);
	}
	// 파일을 쓰기모드 바이너리 파일로 저장 후 파일번호 저장
	if((fp3 = fopen(argv[3], "wb"))==NULL){
		perror("fopen3");
		exit(1);
	}

	long h_size = filesize(fp1)/2; // 전체 파일 사이즈를 반으로 나누어줌
	long count; // 읽을 파일 내용의 크기를 저장하는 변수
	char buf[MAX_BUF]; // 파일 내용을 저장할 변수
	while(h_size>0){ // 파일을 1/2보다 적게 읽었을 경우 계속 반복

		// 읽어야할 남은 파일 크기가 버퍼 최대값보다 많으면 버퍼 최대값 반환
		// 아니면 남은 파일 크기를 반납
		count = (h_size >MAX_BUF) ? MAX_BUF : h_size;
		fread(buf, 1, count,fp1); // count 만큼 파일 내용을 읽음
		fwrite(buf,1, count,fp2); // count 만큼 파일 내용을 씀
		h_size-=count; // 읽어야할 크기에서 읽은 만큼 빼줌
	}
	// 남은 파일 내용을 반복해서 읽어줌
	while((count = fread(buf, 1, MAX_BUF, fp1)) > 0){ 
		fwrite(buf,1,count,fp3); // 읽은 내용을 파일에 씀
	}
	fclose(fp1); //close file
	fclose(fp2);
	fclose(fp3);

	return 0;
}


	
