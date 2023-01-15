#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

int listenConn();
int Strconn();
void sendAck(int idx);
void listenStr();

int Fsock,tsock;
int ackId=0;

int main(int argc, char const *argv[]) {
	struct timespec beg,end;
	clock_gettime(CLOCK_REALTIME,&beg);
    
	listenStr();
    
    clock_gettime(CLOCK_REALTIME,&end);
	long sec = end.tv_sec-beg.tv_sec;
	long nsec = end.tv_nsec-beg.tv_nsec;
	double t = sec+nsec*(1e-9);
	printf("Time Taken:%.20f\n",t);

    return 0;
}

int listenConn(){
	
    int ret;
    char buffer[1000];
    
    //CONN
    struct sockaddr_un svr;
    unlink("ProgSocket");
    int sock = socket(AF_UNIX,SOCK_STREAM,0);
    if(sock<0){
    	perror("Error in creating Socket");
    	exit(EXIT_FAILURE);
    }
    memset(&svr,0,sizeof(struct sockaddr_un));
    svr.sun_family = AF_UNIX;
    strcpy(svr.sun_path,"ProgSocket");
    //binding
    if(bind(sock,(struct sockaddr *) &svr,sizeof(struct sockaddr_un))){
    	perror("Error in building stream Socket");
    	exit(EXIT_FAILURE);
    }
    ret = listen(sock,20);
    if(ret==-1){
    	perror("Issue in Ret of Listen");
    	exit(EXIT_FAILURE);
    }
    // listenStr(sock);
    return sock;
}

void sendAck(int idx){
	char buff[10];
	sprintf(buff,"%d",idx);

	int ret = write(tsock,buff,sizeof(buff));
	if(ret<0){
		perror("While Writing to Ack stream");
		exit(EXIT_FAILURE);
	}
	//printf("ID SENT BY CLIENT = %s\n", buff);	
	printf("ACK sent id:%d\n", idx);
	unlink("ProgSocket");

	if(idx>=50) return;
	listenStr();
}
					


void listenStr(){
	Fsock=listenConn();
	int sock = Fsock;

	char buffer[1000];
	int st=ackId;int curr=0;
    int flag=0;
    int cnt=1; 
    int msgsock;
    for(;;){
    	int rval;
    	msgsock = accept(sock,0,0);

    	if(msgsock==-1){
    		perror("In msgsock");
    		exit(EXIT_FAILURE);
    	}else{
    		
    		bzero(buffer,sizeof(buffer));
    		rval=read(msgsock,buffer,1024);
    		tsock=msgsock;
    		if((rval)<0){
    			perror("Reading Stream MSG");
    		}else if(rval==0){
    			printf("Ending conn\n");
    		}else{
    			printf("ID:%d  STR:",buffer[0]);
    			cnt++;
    			for(int i = 1; i < 12; i++) {
					printf("%c", buffer[i]);
				}
				printf("\n");
				
				if(cnt==6){
					flag=1;
				}
    		}
	    		
	    	if(flag==1){
	    		sendAck(buffer[0]);
	    		break;
	    	}	
	    }	
    }
    
}
