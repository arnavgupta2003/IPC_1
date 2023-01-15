#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>


char randStr[51][12] = {{0}};
int Fsock=0;

int ackId=0;
struct timespec beg,end;

int main(int argc, char const *argv[]) {
	
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
    
    

	int str,ack;
	while(1){
		str=sendStr();
		ack=readAck();
		
		if(ack==1){
			unlink("ProgSocket");

			clock_gettime(CLOCK_REALTIME,&end);
			long sec = end.tv_sec-beg.tv_sec;
			long nsec = end.tv_nsec-beg.tv_nsec;
			double t = sec+nsec*(1e-9);
			printf("Time Taken:%.20f\n",t);

			return 0;
		}
	}

    return 0;
}

int conn(){
	struct sockaddr_un svr;
    int sock;

    sock= socket(AF_UNIX,SOCK_STREAM,0);
    if(sock<0){
    	perror("Error in creating Socket");
    	exit(EXIT_FAILURE);
    }
    svr.sun_family=AF_UNIX;
    strcpy(svr.sun_path,"ProgSocket");

    if(connect(sock,(struct sockaddr *)&svr,sizeof(struct sockaddr_un))<0){
    	close(sock);
    	perror("Connect to stream");
    	exit(EXIT_FAILURE);
    }
    return sock;
}

int sendStr(){
	int ret;
	char buffer[1000];
	

	int lastIdx = ackId+1;

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

		Fsock=conn();
		ret=write(Fsock,randStr[i],sizeof(randStr[i])+1);
		if(ret<0){
			perror("Writing to stream");
			exit(EXIT_FAILURE);
		}
		printf("String sent\n\n");
		sleep(1);
		
	}
	
	printf("FIN With Str Send\n");
	return 0;
}

int readAck(){
	printf("\nWaiting for Acknowledgement from P2\n");
	char buffer[1000];
	int flag=0;
	for(;;){
    	int rval;
		do{
    		bzero(buffer,sizeof(buffer));
    		rval=read(Fsock,buffer,1024);
    		
    		if((rval)<0){
    			perror("Reading Stream MSG");
    			exit(EXIT_FAILURE);
    		}else if(rval==0){
    			printf("Ending conn\n");
    		}else{
    			ackId=atoi(buffer);
    			printf("--> Recieved Ack:%d\n",ackId);
 

				flag=1;
				break;
    		}
    	}while(rval>0);
    	close(Fsock);
    	if(ackId>=50){
    		
    		clock_gettime(CLOCK_REALTIME,&end);
			long sec = end.tv_sec-beg.tv_sec;
			long nsec = end.tv_nsec-beg.tv_nsec;
			double t = sec+nsec*(1e-9);
			printf("Time Taken:%.20f\n",t);

	    	exit(EXIT_SUCCESS);
	    	return 1;
	    }	
    	if(flag==1){
    		break;
    	}	

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


