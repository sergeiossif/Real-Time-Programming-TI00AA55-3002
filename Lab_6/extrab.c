#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

int main (int argc, char** argv) {
	//int fd = open("123.txt", O_WRONY | O_CREAT | O_TRUNC, 0777);
	pid_t pid;
	
	char * filename = argv[1];
	int status;
	int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	char buffer[15];
	sprintf(buffer, "%d", fd);
	
	printf("fd is %d or %s\n", fd, buffer);
	
	pid = fork();
	if(pid == 0){
		int error;
		error = execl("ewritefile.bin", "ewritefile.bin", buffer, (char*) 0);
		if(error == -1){
			printf("Aint working out\n");
		}
	}
	if(pid > 0){
		
		printf("Lets start working\n");

		do {
			printf("working...\n");
			sleep(1);
		} while(waitpid(pid, &status, WNOHANG) == 0);
		
		
		printf("Parent is done\n");
		sleep(1);
		exit(0);
	}
}
