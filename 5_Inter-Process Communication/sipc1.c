#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"
// shared memroy를 이용하여 프로세스간 통신
int main(){
	int shmid;
	char *ptr, *pData;
	int *pInt;
	// shared memory 생성
	if((shmid = shmget(SHM_KEY,SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// shared memory attacth
	if((ptr = shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	// 맨 앞 4바이트는 동기화를 이용할 변수
	pInt = (int*)ptr; // 4byte만큼 변수에 할 당
	while((*pInt)==0) // synchronization
		;
	pData = ptr+sizeof(int); // 4바이트 뒤에 저장할 data
	printf("Received request: %s ....",pData);
	// shared memory에 data를 씀
	sprintf(pData, "This is a reply from %d.",getpid());
	*pInt=0; 
	printf("Replied.\n");

	sleep(1);
	//shared memory 삭제
	if(shmctl(shmid,IPC_RMID,0)<0){
		perror("shmctl");
		exit(1);
	}
	return 0;
}
