#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[]) {
	int fd;
	char buff[5];
	if(*argv[1] == 'a'){
		//buff = "AAAA";
		strcpy(buff, "AAAA");
		}
	else if(*argv[1] == 'b'){
		//buff = "BBBB";
		strcpy(buff, "BBBB");
		}
	else{
		printf("ERROR\n");
	}
	
	
	
	//char buff[] = "BBBB";
	int i;
	fd = open("exlog.txt", O_WRONLY | O_CREAT, 0777);
	
	for(i = 0; i<100000; i++){
		lseek(fd, 0L, SEEK_END);
		write(fd,buff, 4);
	}
	
	
	
	
	

	
	
	
	return 0;
}