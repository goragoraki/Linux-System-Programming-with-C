#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char* argv[]){
	if(argc!=3){
		perror(argv[0]);
		exit(1);
	}

	FILE* fp;
	char* fd = argv[2];
	if((fp=fopen(argv[1],"rb"))==NULL){
		perror("fopen");
		exit(1);
	}
	int len = strlen(fd);
	char buf[len];
	long idx=0;
	int result = 0;
	while(fread(buf, 1,len,fp)){
		fseek(fp,idx,0);
		idx++;
		if(strstr(buf,fd)){
			result+=1;
		}
	}
	printf("%d\n", result);

	return 0;
}
