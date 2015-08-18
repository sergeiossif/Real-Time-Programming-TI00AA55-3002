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

const char *lstrings[5] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE"};


int main (int argc, char** argv) {
	FILE * fp;
	char * filename = argv[1];
	
	int fd = atoi(filename);
	
	//int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
	
	//int letter = 
	
	//char letter = argv[2][0];
	int letter = atoi(argv[2]);
	int nwrite = atoi(argv[3]);
	
	printf("sup, fd is %i, letter is %i\n", fd, letter);
	if (letter >= 0){
		int i;
		for (i=0; i<nwrite; i++)	
			printf("writing: %c , %s\n", letter, lstrings[letter]);
		
	}
	//printf("%s\n", filename);
	//fp = fdopen(fd, "w");
	int i;
	for(i=0;i<nwrite;i++){
		sleep(1);
		
		
		//fprintf(fp, "%s\n", "AAAAA");
	if(write(fd, lstrings[letter], 4)==-1)
		perror("\nWriting error: ");
	else
		printf("\nWrite succeeded.%d\n", i);
		
		//printf("writing...\n");
	}
	printf("terminating\n");
	exit(0);
	
}
