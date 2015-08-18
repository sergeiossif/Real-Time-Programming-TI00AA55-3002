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
			printf("Imma parent, the status is %d\n", WEXITSTATUS(status));
			exit(WEXITSTATUS(status));
		} 
		
		printf("imma child number %d\n", i);
	}
	printf("Imma the last child yo\n");
	exit(0);
	
}
