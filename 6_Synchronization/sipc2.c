#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "prodcons.h"
#include "semlib.h"
// 먼저 값을 만들어 보낸 후 응답을 받음
int main(){
	int shmid;
	char *ptr, *pData;
	int emptyid, mutexid, fullid;
	// shared memory id 만듬
	if((shmid=shmget(SHM_KEY,SHM_SIZE,SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if((ptr=shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	// semaphore id 생성
	if((emptyid=semInit(EMPTY_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	if((fullid=semInit(FULL_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	if((mutexid=semInit(MUTEX_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	// 비어있는 칸이 없을 경우 wait
	if(semWait(emptyid)<0){
		perror("semWait");
		exit(1);
	}
	//mutex -1
	if(semWait(mutexid)<0){
		perror("semWait");
		exit(1);
	}
	// 버퍼 채움
	pData = ptr;
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request......");
	//mutex +1
	if(semPost(mutexid)<0){
		perror("semWait");
		exit(1);
	}
	// 채워진 칸 +1
	if(semPost(fullid)<0){
		perror("semWait");
		exit(1);
	}
	// 비어있는 버퍼칸이 없을 경우 아직 다른 프로세스에서 값을 준비안함 -> wait
	if(semWait(emptyid)<0){
		perror("semWait");
		exit(1);
	}
	printf("Received reply: %s\n", pData);
	// delete semaphore 
	if(semDestroy(emptyid)<0){
		perror("semDestroy");
	}
	if(semDestroy(fullid)<0){
		perror("semDestroy");
	}
	if(semDestroy(mutexid)<0){
		perror("semDestroy");
	}
	// delete shared memory
	if(shmctl(shmid,IPC_RMID,0)<0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
