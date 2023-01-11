#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

char pData[1024]; // 공유 데이터
int Nbuf; // 버퍼에 값이 있는지 여부 판단
pthread_mutex_t Mutex;
pthread_cond_t NotEmpty;

// 값을 받고 그것에 대해 응답함
void Sipc1(void* dummy){
	if(pthread_mutex_lock(&Mutex)<0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if(Nbuf==0){ // 버퍼가 비어있을 경우
		// 기다림
		if(pthread_cond_wait(&NotEmpty, &Mutex)<0){
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	printf("Received request: %s........",pData);
	Nbuf = 0; 
	sprintf(pData,"This is a reply from %ld.",pthread_self());
	printf("Relpied.\n");
	// 버퍼를 받았다고 신호를 보냄
	if(pthread_cond_signal(&NotEmpty)<0){
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}
	if(pthread_mutex_unlock(&Mutex)<0){
		perror("unlock");
		pthread_exit(NULL);
	}
		
}
// 값을 만들고 보낸 후 응답을 받음
void Sipc2(void* dummy){
	if(pthread_mutex_lock(&Mutex)<0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	sprintf(pData, "This is a request from %ld.",pthread_self());
	Nbuf=1; // 버퍼를 채움
	printf("Sent a requset....\n");
	// 버퍼 채웠다고 signal 보냄
	if(pthread_cond_signal(&NotEmpty)<0){
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}
	if(Nbuf==1){ // 버퍼가 1인 경우 아직 저쪽에서 값을 안받아 감
		// 기다림
		if(pthread_cond_wait(&NotEmpty, &Mutex)<0){
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	printf("Received reply: %s\n", pData);
	if(pthread_mutex_unlock(&Mutex)<0){
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
}
// 두 thread가 버퍼에 값을 넣고 가져오고 다시 응답하는 프로그램
int main(){
	pthread_t tid1, tid2;
	int i;
	// cond 생성
	if(pthread_cond_init(&NotEmpty,NULL)<0){
		perror("pthread_cond_init");
		exit(1);
	}
	// mutex 생성
	if(pthread_mutex_init(&Mutex,NULL)<0){
		perror("pthread_mutex_init");
		exit(1);
	}
	//thread create
	if(pthread_create(&tid1,NULL,(void*)Sipc1, (void*)NULL)<0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2,NULL,(void*)Sipc2, (void*)NULL)<0){
		perror("pthread_create");
		exit(1);
	}
	// thread 종료될때까지 기다림
	if(pthread_join(tid1,NULL)<0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2,NULL)<0){
		perror("pthread_join");
		exit(1);
	}
	// cond , mutex 삭제
	if(pthread_cond_destroy(&NotEmpty)<0){
		perror("cond_destroy");
	}
	if(pthread_mutex_destroy(&Mutex)<0){
		perror("mutex_destroy");
	}
	return 0;
}
