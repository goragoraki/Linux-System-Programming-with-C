#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t Mutex, Empty, Full;
int State;
char Data[1024];
// 값을 먼저 받은 후 그에 대한 응답을 보냄
void Sipc1(void* dummy){
	// 버퍼가 비어있다면 wait 버퍼가 개수가 있다면 -1
	if(sem_wait(&Full)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if(sem_wait(&Mutex)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	printf("Received request: %s....", Data);
	sprintf(Data, "This is a reply from %ld.", pthread_self());
	printf("Relpied\n");
	
	if(sem_post(&Mutex)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	// 비어있는 버퍼 칸 개수 +1
	if(sem_post(&Empty)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}

}
void Sipc2(void* dummy){
	// 비어있는 버퍼 칸이 0개면 wait or -1
	if(sem_wait(&Empty)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if(sem_wait(&Mutex)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	// 버퍼 채우고
	sprintf(Data, "This is a request from %ld.",pthread_self());
	printf("Sent a request....\n");
	
	if(sem_post(&Mutex)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	// 버퍼 채운 칸 +1
	if(sem_post(&Full)<0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	
	// 버퍼가 비어있는 개수가 +1 이 되어야 위쪽에서 버퍼를 채우고 넘겨준것이기 때문에
	if(sem_wait(&Empty)<0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	printf("Received reply: %s\n", Data);

}

int main(){
	pthread_t tid1, tid2;
	// 세마코어 생성
	if(sem_init(&Mutex,0,1)<0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&Empty,0,1)<0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&Full,0,0)<0){
		perror("sem_init");
		exit(1);
	}
	//thread create
	if(pthread_create(&tid1,NULL,(void*)Sipc1,(void*)NULL)<0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2,NULL,(void*)Sipc2,(void*)NULL)<0){
		perror("pthread_create");
		exit(1);
	}
	// wait thread terminate
	if(pthread_join(tid1, NULL)<0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL)<0){
		perror("pthread_join");
		exit(1);
	}
	// delete semaphore
	if(sem_destroy(&Mutex)<0){
		perror("sem_destroy");
	}
	if(sem_destroy(&Full)<0){
		perror("sem_destroy");
	}
	if(sem_destroy(&Empty)<0){
		perror("sem_destroy");
	}
	return 0;
}

