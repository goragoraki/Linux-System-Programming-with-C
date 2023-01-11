# Linux System Programming
The goal is to familiarize with using Linux by practicing in a Linux environment.  
Understand the basic concepts of the operating system by directly using various system calls of Linux.  
Finally, Make programs like chatting server & client using various system calls(multi-thread, mutex lock etc).

## Usage
Each folder has a **Makefile**.  
You can compile through commanding **make FileName** on the command line.   
Before executing file, Have to check **argc** variable.  
If you execute mycp(argc of mycp is 2), command like **./mycp A B**.  
All code has comments(korean).  

Basically, it works in the **Ubuntu Linux** environment.  
Most of them work on **Mac**, but it **may not work** if there is a part where the system call is different, such as semaphore.

## Outline
1. File I/O  
2. Files and Directories
3. Processes and Threads  
4. Signals Inter-Process Communication
5. Synchronization
6. Sockets
7. Programs
8. Chatting server & client

## Contents

### 1. File I/O  
* mycp.c - Copy A file to B file (use open()).  
* mycp2.c - Copy A file to B file (use fopen()).  
* split.c - Split the file A in half and write to B and C.  
* merge.c - Merge two separate files into one file

* * *
### 2. Files and Directories

* myrm.c - Remove file or directories
* mylsr.c - Outputs the file name in the directory.  
            If the file to be read is a directory, outputs the file in the directory as well.
* mylsl.c - Works like ls -l 

* * *
### 3. Processes and Threads
* mysystem.c - The operation is the same as the "system" system call, and implemented using fork, execv, and waitpid system call.
* tadd.c - Create two threads to find the sums of 1 to 50 and 51 to 100, respectively. After waiting for each thread to terminate, the main thread adds each sum and outputs them.
* 
* * *
### 4. Signals
* alarm.c - Print "." every one seconds (using signal system call).
* mysleep.c - The operation is the same as the "sleep" system call.
* * *
### 5. Inter-Process Communication
* fifoc.c - Send message to fifos and receive message from fifos (using mkfifo system call).   
* fifos.c - Receive message from fifoc and send message to fifoc (using mkfifo system call).
* synclib.c - Functions for synchronization (using pipe system call).  
* mipc.c - The parent process and the child process alternately increment the number by one (using mmap).
* msgq1.c - Receive message from msgq2 and send message to msgq2 (using message queue).
* msgq2.c - Send message to msgq1 and receive message from msgq1 (using message queue).
* mycp3.c - Copy file A to B (using shared memory). 
* shm.c - Checks where the address of shared memory is located in memory
* mymore.c - Works like ls -l | more (using dup).  
* sipc1.c - Inter Process Communication with sipc2 using shared memory.   
* sipc2.c - Inter Process Communication with sipc1 using shared memory.  
* * *
### 6. Synchronization
producer -> create data and put data in buffer.   
consumer -> get data in buffer. 
* semlib.c - System V -> Semaphore forms.  
* semlib2.c - Make Semaphore (using phread_mutex_lock, pthread_cond_wait).  
* consumer.c - Synchronization with Busy waiting.   
* producer.c - Synchronization with Busy waiting.  
* consumer_s.c - Synchronization with System V.  
* producer_s.c - Synchronization with System V.  

prodcons -> producer + consumer with thread  
* prodcons_t.c - synchronization with busy waiting.  
* prodcons_s.c - synchronization with semaphore.  
* prodcons_m.c - synchronization with pthread_mutex, cond.  
* sipc1.c - Eliminate busy waiting and synchronize through semaphore. (Modify sipc1.c in 5. IPC) 
* sipc2.c - Eliminate busy waiting and synchronize through semaphore. (Modify sipc2.c in 5. IPC)
* sipc3.c - Changed to Multi-thread version using mutexes and condition variables. (Modify sipc1.c, sipc2.c in 5. IPC) 
* sipc4.c - Changed to Multi-thread version using semaphores. (Modify sipc1.c, sipc2.c in 5. IPC) 
* * *
### 7. Sockets
client -> send data to server and received data from server.  
server -> Keep receiving client requests and send data to client.  
* tcpc.c - tcp client. 
* tcps.c - tcp server.
* tcpc_dns.c - client can access to server using server domain.  
* udpc.c - udp client.
* udps.c - udp server.
* select.c - Server can received both tcp client and udp client.
* hw2.c - After the connection is established, create process which handle it exclusively.
* hw3.c - Change select.c to Muti-thread version.  
* * *
### 8. Programs
* del.c - delete one character in string. (using pointer).
* pattern.c - Read binary file and Counts how many times a given word occurs in file.
* size.c - Print the names and sizes of files in the current directory.  
* app.c - Send number to rpc with shared memory and receive calculated number from rpc. Print number.  
* rpc.c - Receive number from app and calculate number(sum up to recevie number). Send number to app.  
* * *
### 9. Chatting server & client
Execute chatc in local : ./chatc "127.0.0.1"
* chats.c - Chatting server select version.
* chatc.c - Chatting client muti-thread version.
* * *
