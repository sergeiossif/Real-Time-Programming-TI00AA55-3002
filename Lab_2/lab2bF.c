#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

char chr_fellow;
int fellow_desc;
int try;



fellow_desc = OpenChatFellow();

for(;;){


	try = read(fellow_desc, &chr_fellow, 1);
	if (try){
		write(STDOUT_FILENO, &chr_fellow, 1);
	}


}
return 0;
}

