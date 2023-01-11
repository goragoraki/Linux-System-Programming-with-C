#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PATH 256
// 디렉토리안의 파일이름을 출력하는데 읽는 파일이 디렉토리일 경우
// 디렉토리안 까지 들어가서 출력
void Doit(char* path){
	DIR* dp;
	struct dirent *dep;
	struct stat statbuf;
	// 디렉토리 열어줌
	if((dp = opendir(path))==NULL){
		perror("opendir");
		exit(1);
	}
	
	printf("%s\n", path); // 현재 디렉토리 path를 출력
	// 디렉토리를 읽고 파일 이름 출력
	while((dep = readdir(dp))!=NULL){
		if(strcmp(".",dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
	}

	rewinddir(dp); // 디렉토리 포인터를 처음으로 바꾸어줌
	char fullpath[MAX_PATH]; // 이동할 디렉토리 path를 저장
	// 디렉토리를 읽음
	while((dep =readdir(dp))!=NULL){
		if(strcmp(".",dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path); // 현재 path를 복사함
		strcat(fullpath, "/"); // "/"를 이어 붙이고
		strcat(fullpath, dep->d_name); // 파일 이름을 붙여줌
		// 파일 정보를 저장 함
		if(lstat(fullpath, &statbuf)<0){
			perror("lstat");
			exit(1);
		}
		// 디렉토리일 경우 안으로 들어가고 path를 파라미터로 줌
		if(S_ISDIR(statbuf.st_mode)){
			Doit(fullpath);
		}
	}
	closedir(dp); //close dir
}

int main(int argc, char* argv[]){
	Doit(".");
	return 0;
}
