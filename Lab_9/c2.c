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



int main(void){
	int fd;
    char * myfifo = "/tmp/namedPipe";
    char buf;
	
	mkfifo(myfifo, 0777);
    /* open, read, and display the message from the FIFO */
    fd = open(myfifo, O_RDONLY);
	printf("reader: going into a cycle\n");
	while(read(fd, &buf, 1) > 0){
		printf("reader: char %c received\n", buf);
		
		if(buf == '6'){
			close(fd);
		}
		
	}		
    
   // printf("Received: %s\n", buf);
    close(fd);

	
	
	
	
	return 0;
}