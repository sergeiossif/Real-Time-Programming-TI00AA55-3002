#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

#define N 7

int totalchar = 0;/*
	void term(int signum){
		printf("SIGTERM received\n");
		exit(totalchar);
	}*/

void term (int signum) {
printf("SIGTEMRN %d\n", totalchar);
exit(1);
}

int main(int argc, char *argv[]) {
int i;
char chr_fellow;
char chr_kb;
int fellow_desc;
int try;
int fd;
int file_flags;
signal(SIGINT, term);
/*
file_flags = fcntl(STDIN_FILENO, F_GETFL); // read current file flags
file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
fcntl(STDIN_FILENO, F_SETFL, file_flags); // write new flags back
//
*/

//setvbuf(STDIN_FILENO, NULL, _IONBF, 1);
fd = open("exlog.txt", O_RDWR | O_CREAT | O_TRUNC , 0777);
pid_t child_pid = fork();

if (child_pid > 0){
	char buffer[40];
	size_t filesize = 0;
	int childwrite;
	
	

	
	while(1){
		try = read(STDIN_FILENO, &chr_kb, 1);
		
		
		if (try  > 0){
			if(chr_kb == 'q' || chr_kb == 'Q'){
				printf("qQ was pressed\n");
				//kill(child_pid, SIGTERM);
				wait(&childwrite);
				childwrite = WEXITSTATUS(childwrite);
				
				printf("Child has written %i chars\n", childwrite);
				printf("Reading the file...\n");
				lseek(fd, 0, SEEK_SET);
				filesize = read(fd, buffer, sizeof(buffer));
					//filesize++;
				printf("The filesize is %i\n", filesize);
				printf("Printing out the file:\n\n");
				
				write(STDOUT_FILENO, buffer, filesize);
				//fflush(NULL);
				exit(0);
				
			}
				
			write(STDOUT_FILENO, &chr_kb, 1);
		}
		
	}
} 

if (child_pid == 0){
	fellow_desc = OpenChatFellow();
/*
	file_flags = fcntl(fellow_desc, F_GETFL); // read current file flags
	file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
	fcntl(fellow_desc, F_SETFL, file_flags); // write new flags back*/
	//fd = open("exlog.txt", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0777);
	int charcount = 0;
	//int totalchar = 0;
	char endline = '\n';
	//signal(SIGTERM, term);

	while(1){
		try = read(fellow_desc, &chr_fellow, 1);
		if (try > 0){
			write(STDOUT_FILENO, &chr_fellow, 1);
			write(fd, &chr_fellow, 1);
			charcount++;
			totalchar++;
			if (charcount > 6){
				write(fd, &endline, 1);
				//printf("\n\n\n %d \n\n\n\n", charcount);
				charcount = 0;
				totalchar++;
			}
		} else {
			
			printf("\nThe child has died\n");
			
			exit(totalchar);
			
			
		}
	}
	
}


close(fellow_desc);
close(STDIN_FILENO);
exit(0);
return 0;
}