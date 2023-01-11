#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType Buf;
pthread_cond_t NotFull; // 버퍼가 Full이 아니라고 알릴 cond
pthread_cond_t NotEmpty; // 버퍼가 Empty가 아니라고 알릴 cond
pthread_mutex_t Mutex; // mutex

// thread sleep 함수
void ThreadUsleep(int usecs){
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	struct timespec ts;
	struct timeval tv;
	if(pthread_cond_init(&cond,NULL)<0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_mutex_init(&mutex,NULL)<0){
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if(pthread_mutex_lock(&mutex)<0){
		perror("mutex_lock");
		exit(1);
	}
	if(pthread_cond_timedwait(&cond,&mutex,&ts)<0){
		perror("pthread_cond_timewait");
		pthread_exit(NULL);
	}
	if(pthread_cond_destroy(&cond)<0){
		perror("cond_destroy");
		pthread_exit(NULL);
	}
	if(pthread_mutex_destroy(&mutex)<0){
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

// 버퍼에 data를 넣는 함수
void Producer(void* dummy){
	int i, data;
	printf("Producer: Start....\n");
	for(i=0;i<NLOOPS;i++){
		// mutex lock
		if(pthread_mutex_lock(&Mutex)<0){
			perror("mutex_lock");
			pthread_exit(NULL);
		}
		while(Buf.counter==MAX_BUF){ // 버퍼가 full 이면
			// NotFull신호가 올때까지 기다리고 mutex를 임시적으로 unlock
			if(pthread_cond_wait(&NotFull, &Mutex)<0){
				perror("cond_wait");
				pthread_exit(NULL);
			}
		}
		// buffer에 data 넣음
		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;
		// NotEmpty 신호 보냄
		if(pthread_cond_signal(&NotEmpty)<0){
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		// mutex unlock
		if(pthread_mutex_unlock(&Mutex)<0){
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}
	
	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}

// 버퍼의 data를 소모
void Consumer(void *dummy){
	int	i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
		// mutex lock
		if (pthread_mutex_lock(&Mutex) < 0)  {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}
		while (Buf.counter == 0)  { // 버퍼가 비어있으면
			// NotEmpty 신호가 올때까지 기다리고 임시적으로 Mutex unlock
			if (pthread_cond_wait(&NotEmpty, &Mutex) < 0)  {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}
		// buffer 소모
		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;
	
		// NotFull 신호를 보냄 
		if (pthread_cond_signal(&NotFull) < 0)  {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		// mutex unlock
		if (pthread_mutex_unlock(&Mutex) < 0)  {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

int main(){
	pthread_t	tid1, tid2;

	srand(0x8888);
	// cond 생성
	if (pthread_cond_init(&NotFull, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_cond_init(&NotEmpty, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	// Mutex 생성
	if (pthread_mutex_init(&Mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}
	// thread: producer 생성
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// thread: consumer 생성
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	//	thread 종료까지 기다림
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);
	// cond, mutex 삭제
	if (pthread_cond_destroy(&NotFull) < 0)  {
		perror("pthread_cond_destroy");
	}
	if (pthread_cond_destroy(&NotEmpty) < 0)  {
		perror("pthread_cond_destroy");
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  {
		perror("pthread_mutex_destroy");
	}
}
