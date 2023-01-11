#define	MY_ID			2
#define	SHM_KEY			(0x9000 + MY_ID)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)
#define	SHM_SIZE	1024
#define	EMPTY_SEM_KEY	(0x5000 + MY_ID)
#define	FULL_SEM_KEY	(0x6000 + MY_ID)
