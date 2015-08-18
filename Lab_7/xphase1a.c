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




// Number of children
#define N 5

//number of writes
#define NUMWRITES 5

// Global variable that indicates how many children have terminated
int n = 0;

// Write signal handler for signal SIGCHLD here. 
// Signal handler increments n.

static void SIGCHLD_handler(int signo){
	printf("SIGCHLD  is caught\n");
	n++;
}

pid_t pid;


int main(void)
{
    // Install signal handler  
    // Open the file data.txt
    int fd = open("data.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
	
	void (*old_handler) (int);
	// Set your own handler and save the old handler
	
	if (signal(SIGCHLD, SIGCHLD_handler) == SIG_ERR)
		fprintf(stderr, "\nCannot set signal handler");
	
	
    // Create N children that start to run a separate 
    // executable file child.exe. Pass the file descriptor,
    // the number of writes and the string that the child
    // program writes to the file.
   
   
	int i = 0;
	// This part is done only by parent but "inherited" by children
	char buffer1[15];
	char buffer2[15];
	char buffer3[15];
	sprintf(buffer1, "%d", fd);
	sprintf(buffer3, "%d", NUMWRITES);
	while (i < 5) {
		pid = fork();
		i++;
		if (pid == 0) {
			printf("Child created, running exec\n");
			sprintf(buffer2, "%d", i+65);
			int error = execl("exphase1b.bin", "exphase1b.bin", buffer1, buffer1, buffer3,  (char*) 0);
			perror("exec fail \n");
			
			//exit(0); // child terminates
		}
		// Parent continues from here after creating a child
	}
	// Parent continues from here after creating all children
	// Tasks of parent process are performed
	//while(wait(NULL) > 0); // wait for all children to terminate
	//exit(0);
   
   
    // Parent process waits for the children in the following loop.
    // Signal causes pause to return
	printf("lets the waiting game begin\n");
    while (n < N) {
        pause();
        printf("Child term %d\n", n);
    }
    // Parent process displays the contents from the file
    return 0;
}
