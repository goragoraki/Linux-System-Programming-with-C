#include <stdio.h>
#include <unistd.h>
#include "semlib.h"
// process 간 semaphore
// system V -> semaphore 형태로

// semaphore 생성
int semInit(key_t key){
	int semid;
	if((semid=semget(key,1,0600|IPC_CREAT))<0){
		perror("semget");
		return -1;
	}
	return semid;
}

// semaphore value 설정
int semInitValue(int semid, int value){
	union semun{
		int val;
	} semun;

	semun.val = value;
	if(semctl(semid,0,SETVAL,semun)<0){
		perror("semctl");
		return -1;
	}

	return semid;
}

// wait semaphore (v>0 -> v-=1 or  v <= 0 wait)
int semWait(int semid){
	struct sembuf semcmd;

	semcmd.sem_num=0;
	semcmd.sem_op=-1;
	semcmd.sem_flg = SEM_UNDO;
	if(semop(semid,&semcmd,1)<0){
		perror("semop");
		return -1;
	}
	return 0;
}
// v<=0 -> return -1 or same semWait
int semTryWait(int semid){
	struct sembuf semcmd;

	semcmd.sem_num=0;
	semcmd.sem_op=-1;
	semcmd.sem_flg=IPC_NOWAIT|SEM_UNDO;
	if(semop(semid,&semcmd,1)<0){
		perror("semop");
		return -1;
	}
	return 0;
}
// v+=1
int semPost(int semid){
	struct sembuf semcmd;

	semcmd.sem_num=0;
	semcmd.sem_op=1;
	semcmd.sem_flg=SEM_UNDO;
	if(semop(semid,&semcmd,1)<0){
		perror("semop");
		return -1;
	}
	return 0;
}
// return value
int semGetValue(int semid){
	union semun{
		int val;
	} dummy;
	return semctl(semid,0,GETVAL,dummy);
}

// delete semaphore
int semDestroy(int semid){
	union semun{
		int val;
	} dummy;
	if(semctl(semid,0,IPC_RMID,dummy)<0){
		perror("semctl");
		return -1;
	}
	close(semid);
	return 0;
}
