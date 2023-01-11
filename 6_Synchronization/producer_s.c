#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "semlib.h"
#include "prodcons.h"
// create buffer with system V
int main(){
	BoundedBufferType *pBuf;
	int shmid, i ,data;
	int emptySemid, fullSemid, mutexSemid;
	// create shared memory id
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// attach shared memory
	if((pBuf=(BoundedBufferType*)shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	
	// init semaphore
	if((emptySemid=semInit(EMPTY_SEM_KEY))<0){
		perror("semInit fail\n");
		exit(1);
	}
	if((fullSemid=semInit(FULL_SEM_KEY))<0){
		perror("semInit fail\n");
		exit(1);
	}
	if((mutexSemid=semInit(MUTEX_SEM_KEY))<0){
		perror("semInit fail\n");
		exit(1);
	}

	srand(0x8888);
	for(i=0;i<NLOOPS;i++){
		// if 비어있는 칸이 없을 경우 wait
		if(semWait(emptySemid)<0){
			perror("semWait");
			exit(1);
		}
		// mutex 자원 없을 경우 wait
		if(semWait(mutexSemid)<0){
			perror("semWait");
			exit(1);
		}
		printf("Producer: Producing an item....\n");
		data = (rand()%100)*10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in+1)%MAX_BUF;
		pBuf->counter++;
		// mutex 자원 반납
		if(semPost(mutexSemid)<0){
			perror("semPost");
			exit(1);
		}
		// 버퍼 차있는 칸 1개 더해줌
		if(semPost(fullSemid)<0){
			perror("semPost");
			exit(1);
		}
		usleep(data);
	}
	sleep(2);
	printf("Producer: Produced %d items.....\n",i);
	printf("Producer: %d items in buffer ......\n", pBuf->counter);
	
	// delete semaphore
	if(semDestroy(emptySemid)<0){
		perror("semDestroy");
	}
	if(semDestroy(mutexSemid)<0){
		perror("semDestroy");
	}
	if(semDestroy(fullSemid)<0){
		perror("semDestroy");
	}
	// delete shared memory
	if(shmctl(shmid, IPC_RMID,0)<0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
