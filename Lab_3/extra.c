#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	size_t len = strlen (argv[1]);
	char *file = malloc(len + 2);
	strcpy(file,argv[1]);

	struct stat sb;
	stat(file, &sb);
	printf("\n");
	if (S_ISREG(sb.st_mode)) 
		printf("This is regular file\n");
	else if (S_ISDIR(sb.st_mode))  
		printf("This is directory file\n");
	else if (S_ISCHR(sb.st_mode)) 
		printf("This is character file\n");           

	printf("File permissions: %#o\n\n\n",sb.st_mode & 0777);
	return 0;
}