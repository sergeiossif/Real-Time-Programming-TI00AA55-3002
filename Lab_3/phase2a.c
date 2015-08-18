#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fd;
	char buff[] = "AAAA";
	int i;
	fd = open("exlog.txt", O_WRONLY | O_CREAT, 0777);
	
	for(i = 0; i<100000; i++){
		lseek(fd, 0L, SEEK_SET);
		write(fd,buff, 4);
	}
	
	
	
	
	

	
	
	
	return 0;
}