#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<sys/un.h>

int main() {

	struct timespec beg,end;
	clock_gettime(CLOCK_REALTIME,&beg);
    int strFfile, ackFfile;

    int ret;
    char buffer[1000];
    char * strFifo = "FIFO_1";
    char * ackFifo = "FIFO_2";
    
    int m1 = mkfifo(strFifo, 0666);
    int m2 = mkfifo(ackFifo, 0666);
    if(m1==-1){
    	perror("strFIFO Err");
    }
    if(m2==-1){
    	perror("ackFIFO Err");
    }


	int maxIdx = 0;
	int curIdx = maxIdx;
	while(1) {
		while(curIdx < maxIdx + 5) {
			
			strFfile = open(strFifo, O_RDONLY);
			ret = read(strFfile, buffer, sizeof(buffer));
			close(strFfile);
			if(ret == -1) {	
				perror("couldn't read");
				exit(EXIT_FAILURE);
			}
			curIdx = buffer[0];
			printf("String Recieved \tID:%d \tString ", curIdx);	
			//printf("STRING SENT BY CLIENT: ");		
			
			for(int i = 1; i < 12; i++) {
				printf("%c", buffer[i]);
			}
			printf("\n");
			
			sprintf(buffer, "%d", curIdx);
			sleep(2);
			// printf("ID SENT BY CLIENT = %d\n", currIdx);	
		}
		//buffer contains the string with the 0th idx as the index of the string
		printf("\n\nSending Acknowledgement to P1\n");
		maxIdx = curIdx;
		ackFfile = open(ackFifo, O_WRONLY);
		ret = write(ackFfile, buffer, sizeof(buffer));
		close(ackFfile);
		printf("Sent Acknowledgement to P1\n\n");
		// break;
		if(maxIdx >= 50){
			clock_gettime(CLOCK_REALTIME,&end);
			long sec = end.tv_sec-beg.tv_sec;
			long nsec = end.tv_nsec-beg.tv_nsec;
			double t = sec+nsec*(1e-9);
			printf("Time Taken:%.20f\n",t);
			exit(EXIT_SUCCESS);
		}
	}
    return 0;
}