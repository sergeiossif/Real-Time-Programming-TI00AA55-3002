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
	int out[N];
	
	int fd = OpenRandomGenerator();
	FILE *file = fdopen(fd, "r");
	int i, count = 0;
	
	for(i=0; i<N; i++){
		fscanf(file, "%d", &out[i]);
		if(out[i]<20)
			count++;
	}
	
	for(i=0; i<N; i++)
	printf("%d\n", out[i]);
	printf("Below 20: %d\n", count);

	return 0;
	
}