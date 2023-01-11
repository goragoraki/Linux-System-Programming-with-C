#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <time.h>
//int형인 숫자를 문자열로 바꾸어서 리턴하는 함수
char* ch_int(int n){
	int i = 0; 
	char s[1024]; //숫자를 저장하는 함수.
	s[i++] = (n%10) + '0'; // 일의자리를 구해 char로 만듬
	while((n/=10)>0) // 십의 자리부터 각 자리들을 char형으로 바꿈
		s[i++] = (n%10) + '0';
	s[i] = '\0'; // 마지막에 문자의 끝을 알려주는 null 넣어줌
	
	// 숫자가 뒤집혀서 저장되어있기 때문에 다시 리버스 시켜줌
	int j = strlen(s) -1;
	char c;
	for(i =0; i<j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	char* rch = s;
	return rch; // 문자로 변환된 숫자 리턴
}
// 유저 이름을 리턴하는 함수
char* GetUid(int id){
	struct passwd* pwd;

	pwd = getpwuid(id);
	return pwd->pw_name;
}
//그룹 이름을 리턴하는 함수
char* GetGid(int id){
	struct group* g;

	g = getgrgid(id);
	return g->gr_name;
}

// ls -l과 같이 작동하는 함수
int main(int argc, char* argv[]){
	DIR* dp;
	//현재 폴더를 열어줌
	if((dp=opendir("."))== NULL){
		perror("opendir");
		exit(1);
	}
	
	struct dirent* dep;
	struct stat statbuf;
	long total = 0;
	// 폴더안 내용을 읽음
	while((dep = readdir(dp))!=NULL){
		if(strcmp(".", dep->d_name) ==0 || strcmp("..", dep->d_name)== 0)
			continue;
		// 각 파일의 상태 정보를 저장해서 블락수를 측정해서 total에 더해줌
		if(lstat(dep->d_name, &statbuf) <0){
			perror("lstat");
			exit(1);
		}
		total += statbuf.st_blocks;
	}
	printf("total %ld\n", total); // total 블락 출력
	rewinddir(dp); // 폴더 포인터를 처음으로 돌려줌

	//폴더안 파일을 하나씩 읽어줌
	while((dep = readdir(dp))!=NULL){
		if(strcmp(".", dep->d_name) ==0 || strcmp("..", dep->d_name)== 0)
			continue;
		char permissions[30] = ""; // 권한을 저장하는 배열
		if(lstat(dep->d_name, &statbuf) <0){
			perror("lstat");
			exit(1);
		}
		
		// 파일의 종류를 분류해서 저장
		if(S_ISREG(statbuf.st_mode))
			strcat(permissions, "-");
		else if(S_ISDIR(statbuf.st_mode))
			strcat(permissions, "d");
		else if(S_ISCHR(statbuf.st_mode))
			strcat(permissions, "c");
		else if(S_ISBLK(statbuf.st_mode))
			strcat(permissions, "b");
		else if(S_ISFIFO(statbuf.st_mode))
			strcat(permissions, "p");
		else if(S_ISLNK(statbuf.st_mode))
			strcat(permissions, "l");
		else if(S_ISSOCK(statbuf.st_mode))
			strcat(permissions, "s");
		else
			strcat(permissions, "w");
		
		// 사용자 권한이 무엇인지  
		if(statbuf.st_mode & S_IRUSR)
			strcat(permissions, "r");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IWUSR)
			strcat(permissions, "w");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IXUSR){
			if(statbuf.st_mode & S_ISUID)
				strcat(permissions, "s");
			else
				strcat(permissions, "x");
		}
		else{
			if(statbuf.st_mode & S_ISUID)
				strcat(permissions, "S");
			else
				strcat(permissions, "-");
		}

		// 그룹 권한이 무엇인지
		if(statbuf.st_mode & S_IRGRP)
			strcat(permissions, "r");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IWGRP)
			strcat(permissions, "w");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IXGRP){
			if(statbuf.st_mode & S_ISGID)
				strcat(permissions, "s");
			else
				strcat(permissions, "x");
		}
		else{
			if(statbuf.st_mode & S_ISGID)
				strcat(permissions, "S");
			else
				strcat(permissions, "-");
		}
		
		// 다른 사용자 권한이 무엇인지
		if(statbuf.st_mode & S_IROTH)
			strcat(permissions, "r");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IWOTH)
			strcat(permissions, "w");
		else
			strcat(permissions, "-");

		if(statbuf.st_mode & S_IXOTH)
			strcat(permissions, "x");
		
		else
			strcat(permissions, "-");
		printf("%s", permissions); // 권한 출력	

		// 링크 숫자가 몇개인지
		int nlnk = statbuf.st_nlink;
		printf("  %d", nlnk);
		
		// 사용자 이름이 무엇인지
		char* user_name = GetUid(statbuf.st_uid);
		int pn = strlen(user_name) + 2;
		printf(" %s", user_name);
		
		// 그룹 이름이 무엇인지
		char* group_name = GetGid(statbuf.st_gid);
		printf("  %s", group_name);
		
		// 파일의 사이즈를 구함
		int fsize = statbuf.st_size;
		printf("%7d", fsize);

		// 최종 수정 날짜 시간 출력
		struct tm *t;
		t = localtime(&statbuf.st_mtime);
		char* month[13] = {"Jan", "Feb", "Mar","Apr","May" ,"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		printf(" %s %d %02d:%02d",month[t->tm_mon], t->tm_mday, t->tm_hour, t->tm_min);
		printf(" %s", dep->d_name); // 파일 이름 출력


		printf("%c", '\n');	
	}
	return 0;

}
