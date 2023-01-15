#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

char randStr[51][12] = {{0}};
key_t shmKey;
int shmid;
int ackId=0;

int main(int argc, char const *argv[]) {
	
	struct timespec beg,end;
	clock_gettime(CLOCK_REALTIME,&beg);


	getStr(randStr);
    printf("\n%s\n", "The below strings are random strings created");
	printf("\n%s\n", "---------START---------");
	for(int i = 1; i <= 50; i++) {
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", randStr[i][j]);
			}
			else printf("%c", randStr[i][j]);
		}
		printf("\n");
	}
	printf("\n%s\n", "---------END---------");
    sleep(3);
    
    shmKey=ftok("ProgSHM",21);
    shmid=shmget(shmKey,1024,0666|IPC_CREAT);

    char *str = (char*) shmat(shmid,(void*)0,0)+1;
    sprintf(str,"%s","----START----");
    
    // printf("%s\n",str);
    shmdt(str);
    int ack,st;
	while(1){
		st=sendStr();
		ack=readAck();
		
		if(ack==1){
			shmctl(shmid,IPC_RMID,NULL);

			clock_gettime(CLOCK_REALTIME,&end);
			long sec = end.tv_sec-beg.tv_sec;
			long nsec = end.tv_nsec-beg.tv_nsec;
			double t = sec+nsec*(1e-9);
			printf("Time Taken:%.20f\n",t);

			return 0;
		}
	}
}

int sendStr(){
	int ret;
	char buffer[1000];
	
	
	int lastIdx = ackId+1;

	printf("Sending strings from Idx %d to %d\n\n\n", lastIdx, lastIdx + 4);
	
	for(int i = lastIdx; i < lastIdx + 5; i++) {
		char *str = (char*) shmat(shmid,(void*)0,0)+1;
		printf("Sending Str: ");
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", randStr[i][j]);
			}
			else printf("%c", randStr[i][j]);
		}
		printf("\n");

		sprintf(str,"%s",randStr[i]);
		shmdt(str);

		printf("String sent\n\n");
		sleep(1);
		
	}
	
	printf("FIN With Str Send\n");
	return 0;
}

int readAck(){

	printf("\nWaiting for Acknowledgement from P2\n");
	sleep(2);
	char *str = (char*) shmat(shmid,(void*)0,0);
	// printf("-->Recieved ACK: %d\n",str);
	printf("-->Recieved ACK: %s\n",str);
	
	ackId=atoi(str);
	shmdt(str);
	if(ackId>=50){
		return 1;
	}

    return 0;
}

void getStr(char strs[51][12]) {        
    srand (time(NULL));                            
    
    
    for(int i = 0; i <= 50; i++) {
		for(int j = 1; j <= 10; j++) {
			strs[i][j] = rand() % 26 + 65;
		}
	}
	for (int j = 0; j <= 50; j++) {
        strs[j][0] = j;
    }
}


