#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argv[1]){
	size_t len = strlen (argv[1]);
	char *filename = malloc(len + 2);
	strcpy(filename,argv[1]);
	}
	char buffer[6];
	
	//FILE *file;
	//file = fopen(stdout, "w");
	
	//setbuf(file, buffer);
	setvbuf(stdout, buffer, _IOFBF, 5);
	
	int i;
	for (i=0; i<13; i++){
		fputc(i+48, stdout);
		usleep(200000);
	}
	
	
	
	//fflush(stdout);
	return 0;
	//exit(0);
	
}