#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <aio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

void handler1(int sig) {
	if (sig == SIGPIPE){
		
		printf("writer: error detected, i shall die\n");
		exit(0);
	}
}

int main(void){
	int fd;
    char * myfifo = "/tmp/namedPipe";
	int i;
	char buf;
	 signal(SIGPIPE, handler1);
	//mkfifo(myfifo, 0777);
	printf("opening a fifo \n");
	fd = open(myfifo, O_WRONLY);
	printf("writer: going into a cycle \n");
	while(1){
		for (i = 0; i < 10; i++){
			buf = '0' + i;
			printf("writer: writing %c to fifo\n", buf);
			write(fd, &buf , sizeof(char));
			
			sleep(1);
		}
	}
	
	
	
	
	return 0;
}