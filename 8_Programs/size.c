#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
void* fc(char* na){
	struct stat buf;
	if(lstat(na, &buf)<0){
		perror("lstat");
		exit(1);
	}
	printf("%s %ld\n",na,buf.st_size);
	pthread_exit(NULL);
}

int main(){
	DIR* dp;
	struct dirent* dep;
	if((dp=opendir("."))==NULL){
		perror("opendir");
		exit(1);
	}
	while(dep=readdir(dp)){
		pthread_t tid;
		if(pthread_create(&tid, NULL,(void*)fc,(void*)dep->d_name)!=0){
			perror("thread");
			exit(1);
		}
	}
	return 0;
}
