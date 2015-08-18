#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>

#define N 7

int main(int argc, char *argv[]) {
	struct rlimit lim;
	pid_t pid, pidout;
	int i = 0;
	int g;
	setvbuf(stdout, NULL, _IONBF, 0);
	if(getrlimit(RLIMIT_NPROC, &lim))
		printf("Error!!!!\n");
	
	printf("Maximum amount of processes: %d\n", lim.rlim_cur);
	while(i<5){
		pid = fork();
		i++;
		if(pid==0){
			//chilstuff
			//printf("number = %d\n", i);
			if(i==1)
				for(g=0;g<5;g++){
					printf("A");
					sleep(1);}
			
			if(i==2)
				for(g=0;g<5;g++){
					printf("B");
					sleep(1);
				}
				
				
			exit(0);
		}
	}
	
	while((pidout = waitpid(-1, NULL, 0)) > 0){
		printf("Child %d terminated\n", pidout);
	}
	exit(0);
	
	//return 0;
	
}