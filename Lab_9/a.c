#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <aio.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>



int main(void){
	
	pid_t pid;
	int fd_arr[2];
	pipe(fd_arr);
	
	pid = fork();
	char tempchar;
	char ttempchar;
	if(pid == 0){
		close(fd_arr[0]);
		for(int i = 0; i < 10; i++){
			ttempchar = '0'+i;
			write(fd_arr[1], &ttempchar, 1);
			sleep(1);
		}
		close(fd_arr[1]);
		exit(0);
	}
	if ( pid > 0) {
		close(fd_arr[1]);
		while(read(fd_arr[0], &tempchar, 1) != 0)
			printf("received %c\n", tempchar);
		
		printf("all is done\n");
		exit(0);
	}
			
}