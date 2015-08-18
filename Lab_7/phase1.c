#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>



static void SIGALRM_handler(int); // forward declaration
pid_t pid;

int main(void) {
	
	void (*old_handler) (int);
	// Set your own handler and save the old handler
	old_handler = signal(SIGALRM, SIGALRM_handler);
	if (old_handler == SIG_ERR)
		fprintf(stderr, "\nCannot set signal handler");

	//signal(SIGINT, old_handler);
	printf("Putting an alarm...\n");
	alarm(5);
	pid = fork();
	pause();
	printf("Exiting\n");
	return 0;
}
static void SIGALRM_handler(int sig_no) {
	if (pid == 0)
		printf("Child alarm\n");
	else if (pid > 0) 
		printf("Parent alarm\n");
	else
		printf("Spooky shit\n");
}