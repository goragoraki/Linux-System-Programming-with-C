#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
// 1부터 50까지 합하는 함수
void Sum50(void* fif){
	int i;
	for(i=1;i<=50;i++)
		*(int*)fif+=i;
	pthread_exit(NULL);
}
// 51부터 100까지 더하는 함수
void Sum100(void* han){
	int i;
	for(i=51;i<=100;i++)
		*(int*)han+=i;
	pthread_exit(NULL);
}

// thread를 생성해서 1부터 100까지 더함
int main(){
	pthread_t tid1;
	pthread_t tid2;
	int fif =0;	// 1~50
	int han =0; // 51~100
	// 50까지 합을 구하는 함수를 실행하는 thread 생성
	// 변수를 공유하기 위해 변수 주소를 파라미터로 줌
	if(pthread_create(&tid1, NULL, (void*)Sum50, (void*)&fif)<0){
		perror("pthread_create");
		exit(1);
	}
	// 51~100 까지 합을 구하는 함수를 실행하는 thread 생성
	if(pthread_create(&tid2, NULL, (void*)Sum100,(void*)&han)<0){
		perror("pthread_create");
		exit(1);
	}
	
	pthread_join(tid1,NULL); // thread1이 끝날때까지 기다림
	pthread_join(tid2,NULL); // thread2가 끝날때까지 기다림
		
	int result = fif + han; // 두 수를 합함
	printf("%d\n", result); // 결과 출력
	return 0;
}

