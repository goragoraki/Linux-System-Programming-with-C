#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"
// consume buffer
int main(){
	BoundedBufferType *pBuf;
	int shmid, i, data;
	// create shared memory id
	if((shmid=shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// shared memory attach
	if((pBuf = (BoundedBufferType*)shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}

	srand(0x9999);
	for(i=0;i<NLOOPS;i++){
		if(pBuf->counter==0){ // if empty buffer
			printf("Consumer: Buffer empty. Waiting....\n");
			while(pBuf->counter==0)
				; // busy waiting
		}
		printf("Consumer: Consuming an item......\n");
		data=pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out+1)%MAX_BUF;
		pBuf->counter--;

		usleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items...\n",i);
	printf("Consumer: %d items in buffer ....\n", pBuf->counter);

	return 0;
}
