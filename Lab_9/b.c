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
	
	pid_t pid;
	struct timespec nano_sleep;
	nano_sleep.tv_sec = 0;
	nano_sleep.tv_nsec = 500000000;
	char tempchar = 'A';
	
	int fd_arr[5][2];
	for (int i=0; i<5; i++)
		pipe(fd_arr[i]);
	
	for (int i = 0; i < 5; ++i){
		nanosleep(&nano_sleep, NULL);
		pid = fork();
		if (pid == 0) {
			printf("im child #%d\n", i);
			/*if (i < 4){
				close(fd_arr[i+1][1]);
				read(fd_arr[i+1][0], &tempchar, 1);
				printf("child #%d is reading %c from the pipe\n", i, tempchar);
			}
			if (i > 0){
				tempchar++;
				close(fd_arr[i][0]);
				printf("child #%d is writing %c into pipe\n", i, tempchar);
				write(fd_arr[i][1], &tempchar, 1);
			}*/
			
				switch (i){
					case 0:
						
						close(fd_arr[1][1]);
						read(fd_arr[1][0], &tempchar, 1);
						printf("child #%d is reading %c from the pipe\n", i, tempchar);
						
						tempchar++;
						printf("the final is %c \n", tempchar);


						break;

					case 1:
						close(fd_arr[2][1]);
						read(fd_arr[2][0], &tempchar, 1);
						printf("child #%d is reading %c from the pipe\n", i, tempchar);
						
						tempchar++;
						close(fd_arr[1][0]);
						printf("child #%d is writing %c into pipe\n", i, tempchar);
						write(fd_arr[1][1], &tempchar, 1);
						
						break;					

					case 2:
						close(fd_arr[3][1]);
						read(fd_arr[3][0], &tempchar, 1);
						printf("child #%d is reading %c from the pipe\n", i, tempchar);
						
						tempchar++;
						close(fd_arr[2][0]);
						printf("child #%d is writing %c into pipe\n", i, tempchar);
						write(fd_arr[2][1], &tempchar, 1);
						
						break;

					case 3:
						close(fd_arr[4][1]);
						read(fd_arr[4][0], &tempchar, 1);
						printf("child #%d is reading %c from the pipe\n", i, tempchar);
						
						tempchar++;
						close(fd_arr[3][0]);
						printf("child #%d is writing %c into pipe\n", i, tempchar);
						write(fd_arr[3][1], &tempchar, 1);
						
						break;

					case 4:
						
						close(fd_arr[4][0]);
						printf("child #%d is writing %c into pipe\n", i, tempchar);
						write(fd_arr[4][1], &tempchar, 1);
						
						break;
					
					default:
						printf("default");
						

				}
				
			
			exit(0);
		}
		
		
	}
	int status;
	//while(wait(&status));
	for(int i = 0; i <5; i++)
		wait(&status);
	return 0;
}