#include <unistd.h> 
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>  
union semun { 
	int val;	 /* value for SETVAL */ 
	struct semid_ds *buf;	 /* buffer for IPC_STAT, IPC_SET */ 
	unsigned short int *array; 	/* array for GETALL, SETALL */ 
	struct seminfo *__buf;	 /* buffer for IPC_INFO */ 
}; 

void main()
{
	int semid = 0;
	union semun sem_union;
	struct sembuf sem_b;
	
	semid = semget((key_t)1234, 1, 0666|IPC_CREAT);
	
	//init 
	sem_union.val = 1;
	if(semctl(semid, 0, SETVAL, sem_union) == -1)
	{
		printf("init error\n");
		exit(1);
	}
	while(1)
	{
		sem_b.sem_num = 0;
		sem_b.sem_op = 2;
		sem_b.sem_flg = SEM_UNDO;
		if(semop(semid, &sem_b, 1) == -1)
		{
			printf("semop error\n");
			exit(1);
		}

		printf("recv a signal\n");
	}
}