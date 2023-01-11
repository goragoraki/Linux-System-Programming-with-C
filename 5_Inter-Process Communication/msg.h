#define MY_ID 2
#define MSG_KEY (0x8000 + MY_ID) // mgsq의 고유 키

#define MSG_REQUEST 1
#define MSG_REPLY 2

typedef struct {
	long type; // reply 인지 request 인지
	char data[1024];
} MsgType;
