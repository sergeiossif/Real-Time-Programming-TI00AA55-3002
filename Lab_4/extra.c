#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define N 7
int main(int argc, char *argv[]) {
	char *filename;
	char verse[10];
	if(argc > 1){
		size_t len = strlen (argv[1]);
		filename = malloc(len + 2);
		strcpy(filename,argv[1]);
	} else {
		printf("Put some arguments, yo\n");
		exit(0);}
	
	FILE *file;
	file = fopen(filename, "r");
	fgets(verse, 10, file);
	
	printf("File descriptor: %d\n", (*file)._fileno);
	printf("File size: %d\n", (*file)._IO_read_end - (*file)._IO_read_base);
	printf("Buffer size: %d\n", (*file)._IO_buf_end - (*file)._IO_buf_base);
	
	short int st = ((*file)._flags &  _IO_UNBUFFERED) + ((*file)._flags &  _IO_LINE_BUF) << 1;
	
	//printf("Buffering mode: %d\n", (*file)._fileno);
	switch(st){
		case 0:
			printf("Buffering mode: Full\n");
			break;
		case 1:
			printf("Buffering mode: None\n");
			break;
		case 2:
			printf("Buffering mode: Line\n");
			break;
	}
	return 0;
	
}