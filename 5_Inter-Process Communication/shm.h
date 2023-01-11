#define MY_ID 2
#define SHM_KEY (0x9000 + MY_ID) // shared memory 고유 key 
#define SHM_SIZE 1024 // memory size
#define SHM_MODE (SHM_R|SHM_W|IPC_CREAT)
