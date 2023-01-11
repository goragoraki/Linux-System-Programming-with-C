#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W)

char Array[ARRAY_SIZE]; // 전역변수는 data에 저장됨
// shared memory의 주소가 어디에 있는지 출려고하는 함수
int main(){
	int shmid;
	char *ptr, *shmptr;
	// 동적 할당(heap) 에 저장됨
	if((ptr=(char*)malloc(MALLOC_SIZE)) == NULL){
		perror("malloc");
		exit(1);
	}
	// local 변수는 stack에 저장됨
	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	// shared memory는 stack 과 heap 사이에 저장됨
	if((shmptr= shmat(shmid,0,0)) == (void*)-1){
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %p to %p \n", &Array[0], &Array[ARRAY_SIZE]);
	printf("Stack around %p\n", &shmid);
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE);
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE);
	
	// shared memory 삭제
	if(shmctl(shmid, IPC_RMID,0)<0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
