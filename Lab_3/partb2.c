#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int print_flags(int fd){
	int flags = fcntl(fd, F_GETFL);
	int access_mode = flags & O_ACCMODE;
	
	printf("Access mode: ");
	if(access_mode == O_RDONLY)
		printf("Read only\n");
	else if(access_mode == O_WRONLY)
		printf("Write only\n");
	else if(access_mode == O_RDWR)
		printf("Read and Write\n");

	printf("O_NONBLOCK: ");
	if (flags & O_NONBLOCK)
		printf("ON\n");
	else
		printf("OFF\n");
	
	printf("O_APPEND: ");
	if (flags & O_APPEND)
		printf("ON\n");
	else
		printf("OFF\n");
	
	
	return flags;
}


int main(int argc, char *argv[]) {
	int fd;
	char buff[] = " \0";
	fd = open("/dev/tty", O_RDONLY);


	print_flags(fd);
	
	
	if(read(fd, buff, 1)==-1)
		perror("Read error: ");
	else 
		printf("Read succeeded: %s\n", buff);
	
	

	if(write(fd, "A", 1)==-1)
		perror("\nWriting error: ");
	else
		printf("\nWrite succeeded.\n");
	
	close(fd);
	
	
			
	
	
	return 0;
}