#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"
#include <unistd.h>
#include <stdlib.h>
#define NLOOPS 10
#define SIZE sizeof(long)
// 값을 리턴하고 하나 증가 시킴
int update(long *ptr){
	return ((*ptr)++);
}
// 부모 프로세스와 자식프로세스가 번갈아가며 숫자 한개를 증가시킴
int main(){
	int fd, i, counter;
	pid_t pid;
	caddr_t area; // shared memory 주소

	// zero라는 파일을 열어줌
	if((fd=open("/dev/zero", O_RDWR))<0){
		perror("open");
		exit(1);
	}
	// zero라는 파일과 mmap 시켜 shared memory를 만듬
	if((area=mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==(caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	close(fd);
	TELL_WAIT(); // pipe 두 개를 만듬
	if((pid=fork())<0){ // fork
		perror("fork");
		exit(1);
	}else if(pid>0){ // 부모 프로세스
		for(i=0;i<NLOOPS;i+=2){
			// 값을 하나 증가시킴
			if((counter=update((long*)area))!=i){
				fprintf(stderr,"Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter = %d\n",counter);
			TELL_CHILD(); // 자식에게 값 보냄
			WAIT_CHILD(); // 자식 값 기다림
		}
	}else{
		for(i=1;i<NLOOPS;i+=2){
			WAIT_PARENT(); // 부모 값 기다림
			// 값 하나 증가시킴
			if((counter=update((long*)area))!=i){
				fprintf(stderr,"Counter mismatch\n");
				exit(1);
			}
			printf("Child: counter = %d\n",counter);
			TELL_PARENT(); // 부모에게 값 보냄
		}
	}
	return 0;
}
