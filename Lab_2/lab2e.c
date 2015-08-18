#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
int main(int argc, char *argv[]) {
time_t current;

current = time(NULL);
struct tm tm = *localtime(&current);

for(;;){


current = time(NULL);
struct tm tm = *localtime(&current);	
printf("%d.%d.%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
sleep(1);


}
return 0;
}

