#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include "shm.h"
#include "semlib.h"
int shmid;
int
semInit(key_t key)
{
    int     semid;

    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }

    return semid;
}
 
int
semInitValue(int semid, int value)
{
    union semun {
            int     val;
    } semun;

    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
		perror("semctl");
        return -1;
    }
 
    return semid;
}
 
int
semWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}
 
int
semTryWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semPost(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semGetValue(int semid)
{
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy);
}
 
int
semDestroy(int semid)
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
		perror("semctl");
        return -1;
    }
    close(semid);
 
    return 0;
}int sema1, sema2;
void SigInt(int errno){
	if (semDestroy(sema1) < 0)  {
		perror("semdestroy");
	}
	if (semDestroy(sema2) < 0)  {
		perror("semdestroy");
	}
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}
int main(){
	char* ptr;
	int* pInt;
	if (signal(SIGINT, SigInt) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	if((shmid=shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	if((ptr=shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	if ((sema1 = semInit(EMPTY_SEM_KEY)) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	if ((sema2 = semInit(FULL_SEM_KEY)) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	if (semInitValue(sema1, 0) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	if (semInitValue(sema2, 0) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	while(1){
		if (semWait(sema1) < 0)  {
			perror("semInit failure");
			exit(1);
		}
		pInt=(int*)ptr;
		int num = *pInt;
		int sum=0;
		for(int i =0; i<=num;i++){
			sum+=i;
		}
		*pInt=sum;
		printf("%d\n",*pInt);

		if (semPost(sema2) < 0)  {
			perror("semInit failure");
			exit(1);
		}
	}
	return 0;
}
