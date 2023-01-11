#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char* argv[]){
	int fdin, fdout; // 복사할 파일, 복사 받을 파일
	char *src, *dst;
	struct stat statbuf;

	if(argc!=3){
		perror(argv[0]);
		exit(1);
	}

	if((fdin=open(argv[1], O_RDONLY))<0){ // 파일 open
		perror("open");
		exit(1);
	}

	// 복사 받을 파일 만듬
	if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644))<0){
		perror("open");
		exit(1);
	}
	
	// 복사할 파일의 stat 가져옴
	if(fstat(fdin, &statbuf)<0){
		perror("fstat");
		exit(1);
	}
	// 복사 할 사이즈 만큼 파일 포인터를 옮겨 생성함
	if(lseek(fdout, statbuf.st_size-1, SEEK_SET)<0){
		perror("lseek");
		exit(1);
	}
	// 마지막 NULL 넣어줌
	write(fdout,"",1);
	
	// map을 이용하여 virtual address와 파일을 연결함
	// 주소를 포인터로 받음
	if((src=mmap(0,statbuf.st_size,PROT_READ,MAP_SHARED,fdin,0))==(caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	if((dst=mmap(0,statbuf.st_size,PROT_WRITE,MAP_SHARED,fdout,0))==(caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	// 메모리에 있는 내용을 복사
	memcpy(dst,src,statbuf.st_size);
	close(fdin); // 파일 close
	close(fdout);
	return 0;
}
