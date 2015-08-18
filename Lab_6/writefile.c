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
	FILE * fp;
	char * filename = argv[1];
	//int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	int fd = atoi(filename);
	
	//printf("%s\n", filename);
	int i;
	fp = fdopen(fd, "w");
	
	for(i=0;i<5;i++){
		sleep(1);
		fprintf(fp, "%s\n", "AAAAA");
		printf("writing...\n");
	}
	exit(0);
	
}
