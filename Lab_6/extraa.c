#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main (void) {
	pid_t pid, pid2;
	int status;
	int i;
	
	for (i = 0; i<5; i++){
		pid = fork();
		if (pid > 0){//parent
			wait(&status);
			printf("Child's exiting status is %d\n", WEXITSTATUS(status));
			exit(WEXITSTATUS(status)+1);
		} 
		
		printf("Child number %d\n", i);
	}
	printf("The last child\n");
	sleep(1);
	exit(0);
	
}
