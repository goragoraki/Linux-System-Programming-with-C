#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "shm.h"
#include <stdio.h>
#include "semlib.h"
 
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
}
int Calc(int number){
	int shmid;
	char* ptr;
	int *pInt;
	int sema1, sema2,result;
	if((shmid=shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
		perror("shmget");
		exit(1);
	}
	if((ptr=shmat(shmid,0,0))==(void*)-1){
		perror("shmat");
		exit(1);
	}
	pInt=(int*)ptr;
	*pInt = number;
	if ((sema1 = semInit(EMPTY_SEM_KEY)) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	if (semPost(sema1) < 0)  {
		perror("semInit failure");
		exit(1);
	}

	if ((sema2 = semInit(FULL_SEM_KEY)) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	if (semWait(sema2) < 0)  {
		perror("semInit failure");
		exit(1);
	}
	result=*pInt;
	return result;
}

int main(int argc, char* argv[]){
	int num, result;
	num = atoi(argv[1]);
	result = Calc(num);
	printf("Calc = %d\n",result);
	return 0;
}
