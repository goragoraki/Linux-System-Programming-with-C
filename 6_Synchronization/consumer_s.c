#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "semlib.h"
#include "prodcons.h"
//consume buffer
int main(){
	BoundedBufferType *pBuf;
	int shmid, data, i;
	int emptySemid, mutexSemid, fullSemid;
	// create shared memory id
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// attach shared memory
	if((pBuf=(BoundedBufferType*)shmat(shmid,0,0))<0){
		perror("shmat");
		exit(1);
	}
	// 세마포어 생성
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
	//세마포어 value 설정
	if(semInitValue(emptySemid,MAX_BUF)<0){
		perror("semInitValue");
		exit(1);
	}
	if(semInitValue(mutexSemid,1)<0){
		perror("semInitValue");
		exit(1);
	}
	if(semInitValue(fullSemid, 0)<0){
		perror("semInitValue");
		exit(1);
	}
	
	srand(0x9999);
	for(i=0;i<NLOOPS;i++){
		// 차 있는 버퍼가 0일 경우 wait
		if(semWait(fullSemid)<0){
			perror("semWait");
			exit(1);
		}
		//mutex 자원 한개 소비 없으면 wait
		if(semWait(mutexSemid)<0){
			perror("semWait");
			exit(1);
		}
		printf("Consumer: Consuming an item....\n");
		data=pBuf->buf[pBuf->out].data;
		pBuf->out=(pBuf->out+1)%MAX_BUF;
		pBuf->counter--;
		// mutex 자원 돌려줌
		if(semPost(mutexSemid)<0){
			perror("semPost");
			exit(1);
		}
		// 비어있는 버퍼 칸 한개 추가
		if(semPost(emptySemid)<0){
			perror("semPost");
			exit(1);
		}
		usleep((rand()%100)*10000);
	}
	printf("Consumer: Consumed %d items ....\n",i);
	printf("Consumer: %d items in buffer.....\n",pBuf->counter);
	return 0;
}
