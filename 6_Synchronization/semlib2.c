#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

// create semaphore
int sem_init(sem_t *sem, int pshared, int value){
	if (pshared)  {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}
	
	if (pthread_mutex_init(&sem->mutex, NULL) < 0)
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0)
		return -1;

	// 사용 가능한 자원의 초기값
	sem->sval = value;

	return 0;
}

// wait semaphore
int sem_wait(sem_t *sem){
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;
		// value가 0일 경우 기다리다 0보다 커지면 락을 품
		while (sem->sval == 0)  {
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  {
				if (pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		sem->sval--;

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}
// value가 0이면 -1 리턴 다른 경우 wait가 똑같이 작동
int sem_trywait(sem_t *sem){
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;
	// value 가 0이면 -1 리턴
	if (sem->sval == 0)  {
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}
// post
int sem_post(sem_t *sem){
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;
		//wait 상태에 있던 것에 signal
		if (pthread_cond_signal(&sem->cond) < 0)  {
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}

	sem->sval++;

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}
// value return
int sem_getvalue(sem_t *sem, int *sval){
	*sval = sem->sval;

	return 0;
}
// destroy semaphore
int sem_destroy(sem_t *sem){
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}

