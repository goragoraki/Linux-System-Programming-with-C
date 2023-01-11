#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
// shared memory를 이용한 프로세스간 통신
int main(){
	int shmid;
	char *ptr, *pData;
	int *pInt;
	//shared memory id를 받아줌
	if((shmid=shmget(SHM_KEY,SHM_SIZE,SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	//shared memory attatch로 시작 point 받아줌 
	if((ptr=shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	pInt = (int*)ptr; // 동기화를 위한 앞 4바이트 받아줌
	pData = ptr+sizeof(int); // 데이터 부분 포인터
	// shared memory에 data를 씀
	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1; // 동기화 변수 1로
	printf("Sent a request......");

	while((*pInt) == 1) // 기다림
		;
	// 받은 데이터 출력 
	printf("Received reply: %s\n", pData);

	return 0;
}
