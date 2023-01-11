#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "prodcons.h"
#include "semlib.h"
// 값을 받고 그에대한 응답 보냄
// 프로세스 간 통신
int main(){
	int shmid;
	char *ptr, *pData;
	int emptySemid, mutexSemid, fullSemid;
	// shared memory 만듬
	if((shmid = shmget(SHM_KEY,SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// attach shared memory
	if((ptr = shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	// semaphore 생성
	if((emptySemid=semInit(EMPTY_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	if((fullSemid=semInit(FULL_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	if((mutexSemid=semInit(MUTEX_SEM_KEY))<0){
		perror("semInit");
		exit(1);
	}
	// semaphore value 설정
	if(semInitValue(emptySemid,1)<0){
		perror("semInitValue");
		exit(1);
	}
	if(semInitValue(fullSemid,0)<0){
		perror("semInitValue");
		exit(1);
	}
	if(semInitValue(mutexSemid,1)<0){
		perror("semInitValue");
		exit(1);
	}
	// 버퍼 채워진 칸이 0이면 wait
	if(semWait(fullSemid)<0){
		perror("semWait");
		exit(1);
	}
	// critical section 보호
	if(semWait(mutexSemid)<0){
		perror("semWait");
		exit(1);
	}
	pData = ptr;
	printf("Received request: %s ....",pData);
	sprintf(pData, "This is a reply from %d.",getpid());
	printf("Replied.\n");
	// mutex +1
	if(semPost(mutexSemid)<0){
		perror("semWait");
		exit(1);
	}
	// 비어있는칸 +1
	if(semPost(emptySemid)<0){
		perror("semWait");
		exit(1);
	}

	return 0;
}
