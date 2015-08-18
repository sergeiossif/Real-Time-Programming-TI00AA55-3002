#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <aio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>

static int arrayint;

struct timespec ttime1, ttime2;
int delta_t(struct timespec *, struct timespec *, struct timespec *);

float standard_deviation(int data[], int n);


void handler(int signo, siginfo_t *siginfo, void *context){
	clock_gettime(CLOCK_REALTIME, &ttime1);
	printf("handler in action\n");
	arrayint++;
	
	printf("handler: time is %ld s, %ld ns\n", ttime1.tv_sec, ttime1.tv_nsec);
	
}


int main(void){
	
	
	int n;
    int times[10000];
	sigset_t mask, oldmask;
    struct timespec real_time_res;
    union sigval value;
	value.sival_int = 5;
	pid_t pid;
	int i;
	int fd[2];
	pipe(fd);
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	//struct timespec time;
    clock_getres(CLOCK_REALTIME, &real_time_res);
    printf("Real time clock resolution is  :%ld s %ld ns\n", 
		real_time_res.tv_sec, real_time_res.tv_nsec );
		
	pid = fork();
	
	 if (pid == 0){//child
		printf("childish stuff\n");
		
		close(fd[1]);
		
		sigaction(SIGRTMAX, &sa, NULL);
		sigemptyset(&mask);
		sigdelset(&mask, SIGRTMAX);
		
		printf("suspending...\n");
		sigsuspend(&mask);
		printf("Signal handled\n");
		read(fd[0], &ttime2, sizeof(ttime2));
		struct timespec result;
		delta_t(&result, &ttime2, &ttime1);
		printf("child: time2 is %ld s, %ld ns\n", ttime2.tv_sec, ttime2.tv_nsec);

		printf("child: result is %ld s, %ld ns\n", result.tv_sec, result.tv_nsec);
		
		times[arrayint] = result.tv_nsec;
		
		printf("child: times[%d] = %ld\n", arrayint, times[arrayint]);
		
		exit(0);
	} else if (pid > 0) { //parent
		sleep(1);
		
		close(fd[0]);
		
		clock_gettime(CLOCK_REALTIME, &ttime2);
		for(i = 0; i<3; i++){
		sigqueue(pid, SIGRTMAX, value);}
		
		write(fd[1], &ttime2, sizeof(ttime2));
		printf("parent: time2 is %ld s, %ld ns\n", ttime2.tv_sec, ttime2.tv_nsec);
		printf("signal sent\n");
		sleep(5);
		
	}
	
	return 0;
}








int delta_t(struct timespec *interval, struct timespec *begin, struct timespec *now)
{
interval->tv_nsec = now->tv_nsec - begin->tv_nsec; /* Subtract 'decimal fraction' first */
if(interval->tv_nsec < 0 ){
interval->tv_nsec += 1000000000; /* Borrow 1sec from 'tv_sec' if subtraction -ve */
interval->tv_sec = now->tv_sec - begin->tv_sec - 1; /* Subtract whole number of seconds and return 1 */
return (1);
}
else{
interval->tv_sec = now->tv_sec - begin->tv_sec; /* Subtract whole number of seconds and return 0 */
return (0);
}
} 



float standard_deviation(int data[], int n)
{
    float mean=0.0, sum_deviation=0.0;
	long int buffer;
    int i;
    for(i=0; i<n;++i)
    {
        buffer+=data[i];
    }
    mean= (float) buffer/n;
    for(i=0; i<n;++i)
    sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);           
}