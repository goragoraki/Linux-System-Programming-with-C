#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void del(char *str, char c){
	char* dst = str;
	while(*str){
		if(*str != c){
			*dst++ = *str;
		}
		str++;
	}
	*dst='\0';
}

int main(){
	char str[20] = "Hello";
	del(str, 'l');
	puts(str);
	return 0;
}
