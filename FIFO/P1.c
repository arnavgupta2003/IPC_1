#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<sys/un.h>

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

int main() {
    
	struct timespec beg,end;
	clock_gettime(CLOCK_REALTIME,&beg);
    int ret;
    char buffer[1000];
    // FIFO file path
    char * strFifo = "FIFO_1";
    char * ackFifo = "FIFO_2";
    
    char randStr[51][12] = {{0}};

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

	int lastIdx = 1;
	int strFFile, ackFfile;
	
	while(1) {
		printf("Sending strings from Idx %d to %d\n\n\n", lastIdx, lastIdx + 4);
		
		for(int i = lastIdx; i < lastIdx + 5; i++) {
			
			printf("Sending Str: ");
			for(int j = 0; j < 12; j++) {
				if(j == 0) {
					printf("%d ", randStr[i][j]);
				}
				else printf("%c", randStr[i][j]);
			}
			printf("\n");


			strFFile = open(strFifo, O_WRONLY);
			ret = write(strFFile, randStr[i], sizeof(randStr[i]) + 1);
			sleep(1);	
			close(strFFile);

			printf("String sent\n\n");
		}
		printf("\nReading Acknowledgement from P2\n");
		//Reading Ack
		ackFfile = open(ackFifo, O_RDONLY);
		ret = read(ackFfile, buffer, sizeof(buffer));
		close(ackFfile);

		if(ret==-1){
    		perror("ackFIFO Read issue");
    		exit(EXIT_FAILURE);
    	}

		int finalIdx = atoi(buffer);
 		printf("Acknowledgement Recieved for id till:%s\n\n", buffer);

		lastIdx = finalIdx + 1;
		if(lastIdx >= 50){ 
			clock_gettime(CLOCK_REALTIME,&end);
			long sec = end.tv_sec-beg.tv_sec;
			long nsec = end.tv_nsec-beg.tv_nsec;
			double t = sec+nsec*(1e-9);
			printf("Time Taken:%.20f\n",t);
			exit(EXIT_SUCCESS);
		};
	}

    return 0;
}