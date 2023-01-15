#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

int listenConn();
int Strconn();
void sendAck(int idx);
void listenStr();


int ackId=0;

key_t shmKey;
int shmid;
struct timespec beg,end;

int main(int argc, char const *argv[]) {
	
	clock_gettime(CLOCK_REALTIME,&beg);
	
	shmKey=ftok("ProgSHM",21);
    shmid=shmget(shmKey,1024,0666|IPC_CREAT);


    printf("Syncing...\n");
	char *str = (char*) shmat(shmid,(void*)0,0)+1;
	while(1){
		// printf(":%s:\n",str);
		// memmove(str,str+1,strlen(str));
		// printf(":%s:\n",str);
		if(strcmp(str,"----START----")==0){
			printf("Synced.\n");
			shmdt(str);
			break;
		}
	}
	listenStr();
    
    return 0;
}


void sendAck(int idx){

	char *str = (char*) shmat(shmid,(void*)0,0);
	sprintf(str,"%d",idx);
	shmdt(str);

	printf("ACK sent id:%d\n", idx);
	ackId=idx;
	sleep(2);
	listenStr();
}
					


void listenStr(){
	
	if(ackId>=50){

		clock_gettime(CLOCK_REALTIME,&end);
		long sec = end.tv_sec-beg.tv_sec;
		long nsec = end.tv_nsec-beg.tv_nsec;
		double t = sec+nsec*(1e-9);
		printf("Time Taken:%.20f\n",t);

    	exit(EXIT_SUCCESS);
    	
    }
	char buffer[1000];
    for(int i=1;i<=5;i++){
    	sleep(1);
    	char *str = (char*) shmat(shmid,(void*)0,0)+1;
		
		printf("ID:%d  STR:",str[0]);
		for(int i = 1; i < 12; i++) {
			printf("%c", str[i]);
		}
		printf("\n");
		ackId=(str[0]);
    }
    sendAck(ackId);
    
    
}
