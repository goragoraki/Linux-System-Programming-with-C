#define MAX_FIFO_NAME 32
#define SERV_FIFO "./.fifo" // 서버 fifo 파일 이름 정의
typedef struct{
	char returnFifo[MAX_FIFO_NAME]; // 각 cilent fifo 파일 이름 
	char data[128]; // data
} Msgtype;

