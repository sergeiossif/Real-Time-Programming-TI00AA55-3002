#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
int i;
char chr_fellow;
char chr_kb;
int fellow_desc;
int try;

int file_flags;

fellow_desc = OpenChatFellow();

file_flags = fcntl(fellow_desc, F_GETFL); // read current file flags
file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
fcntl(fellow_desc, F_SETFL, file_flags); // write new flags back


file_flags = fcntl(STDIN_FILENO, F_GETFL); // read current file flags
file_flags = file_flags | O_NONBLOCK; // add O_NONOBLOCK bit
fcntl(STDIN_FILENO, F_SETFL, file_flags); // write new flags back



for(;;){


	try = read(fellow_desc, &chr_fellow, 1);
	if (try != -1){
		write(STDOUT_FILENO, &chr_fellow, 1);
	}

	try = read(STDIN_FILENO, &chr_kb, 1);
	if (try != -1){
		write(STDOUT_FILENO, &chr_kb, 1);
	}

}
return 0;
}

