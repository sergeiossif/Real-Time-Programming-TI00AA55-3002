#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main (void) {
	 pid_t pid, pid2;
	 char buf[20];
	 printf("Enter input (n = next, other input terminates):");
	 fgets(buf, 20, stdin);
	 while(buf[0] == 'n') {
		 pid = fork();
		 if (pid < 0) {
			 perror("Fork:");
			 exit(1);
		 }
		 if (pid == 0) {
			 pid2 = fork();
			 if(pid2>0){
				 exit(0);
			 } else if(pid2==0) {
				 
				 sleep(5); // This represents something real work
				 exit(0); // that is done for the client
			 }
		 }
		 wait();
		 printf("Enter input (n = next, other input terminates):");
		 fgets(buf, 20, stdin);
	}
}
