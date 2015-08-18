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

#include <limits.h>

#ifdef VERBOSE
#define VB(x) x
#else
#define VB(x) do { } while (0)
#endif

#define NMEASURES 10000
static int arrayint;

static int flag = 0;

struct timespec ttime1, ttime2;
int delta_t(struct timespec *, struct timespec *, struct timespec *);




struct stats {
	float std;
	float mean;
	int min;
	int max;
};


struct stats standard_deviation(int data[], int n);


void handler(int signo, siginfo_t *siginfo, void *context){
	clock_gettime(CLOCK_REALTIME, &ttime1);
	VB(printf("handler in action\n"));
	
	
	VB(printf("handler: time is %ld s, %ld ns\n", ttime1.tv_sec, ttime1.tv_nsec));
	
}


void parhandler(int signo, siginfo_t *siginfo, void *context){

	exit(0);
	
}

int main(void){
	
	VB(printf("Verbose mode\n"));
	int n;
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
    clock_getres(CLOCK_REALTIME, &real_time_res);
    printf("Real time clock resolution is  :%ld s %ld ns\n", 
		real_time_res.tv_sec, real_time_res.tv_nsec );
		
	pid = fork();
	
	 if (pid == 0){//child
		VB(printf("childish stuff\n"));
		
		int times[NMEASURES];
		
		
		close(fd[1]);
		
		sigaction(SIGRTMAX, &sa, NULL);
		sigemptyset(&mask);
		sigdelset(&mask, SIGRTMAX);
		
		while(arrayint < NMEASURES){
			VB(printf("child: suspending...\n"));
			sigsuspend(&mask);
			
			VB(printf("child: Signal handled\n"));
			read(fd[0], &ttime2, sizeof(ttime2));
			struct timespec result;
			
			result.tv_sec = ttime1.tv_sec - ttime2.tv_sec;
			result.tv_nsec = ttime1.tv_nsec - ttime2.tv_nsec;
			
			
			
			
			VB(printf("child: time2 is %ld s, %ld ns\n", ttime2.tv_sec, ttime2.tv_nsec));
			VB(printf("child: result is %ld s, %ld ns\n", result.tv_sec, result.tv_nsec));
			
			
			
			times[arrayint] = result.tv_nsec / 1000;
			
			if (arrayint % (NMEASURES / 10) == 0){ 
				printf(".");
				fflush(0);
			}
			
			VB(printf("child: times[%d] = %ld\n", arrayint, times[arrayint]));
			
			arrayint++;
			
		} 
			
		printf("That's literally too much!\n");
		
		struct stats result = standard_deviation(times, arrayint);
	
		printf ( "Signals received %d, mean %.2fus, std %.2fus, min %dus, max %dus\n", arrayint, result.mean, result.std, result.min, result.max);
		
		sigqueue(getppid(), SIGTERM, value);
		exit(0);
		
		
	} else if (pid > 0) { //parent
		sleep(1);
		int sleepytime;
		sa.sa_sigaction = parhandler;
		
		sigaction(SIGTERM, &sa, NULL);
		sigemptyset(&mask);
		sigdelset(&mask, SIGRTMAX-1);
		
		close(fd[0]);
		
		clock_gettime(CLOCK_REALTIME, &ttime2);
		//for(i = 0; i<3 && flag == 1; i++){
		
		sigqueue(pid, SIGRTMAX, value);
		write(fd[1], &ttime2, sizeof(ttime2));
		VB(printf("parent: time2 is %ld s, %ld ns\n", ttime2.tv_sec, ttime2.tv_nsec));
		VB(printf("parent: signal sent\n"));
		
		while(1){
			VB(printf("parent: Zzz...\n"));
			sleepytime = 1 + (32-1) * (double)rand() / (double)RAND_MAX + 0.5;
			VB(printf("sleepytime is %ld\n", sleepytime));
			usleep(sleepytime*1000);
			//sigsuspend(&mask);
			VB(printf("parent: Papa is awake\n"));
			clock_gettime(CLOCK_REALTIME, &ttime2);
			sigqueue(pid, SIGRTMAX, value);
			write(fd[1], &ttime2, sizeof(ttime2));
			VB(printf("parent: time2 is %ld s, %ld ns\n", ttime2.tv_sec, ttime2.tv_nsec));
			VB(printf("parent: loop signal sent\n"));
			

		}
		sleep(5);
		
	}
	
	return 0;
}











struct stats standard_deviation(int data[], int n)
{
	VB(printf("im int std function\n"));
    float mean=0.0, sum_deviation=0.0;
	 long long int buffer = 0;
    int i;
	int min = INT_MAX, max = 0;
	struct stats rstats;
    for(i=0; i<n;++i)
    {
		
        buffer+=data[i];
		VB(printf("data[%d] is %d, buffer is %d\n", i, data[i], buffer));
		
		min = data[i] < min ? data[i] : min;
		max = data[i] > max ? data[i] : max;
    }
	
    mean = ((float) buffer)/n;
	
    VB(printf("Buffer is %ld, mean is %f\n", buffer, mean));
	for(i=0; i<n;++i){
		sum_deviation+=(data[i]-mean)*(data[i]-mean);
		VB(printf("sum dev is %f\n", sum_deviation));
	}
	rstats.mean = mean;
	rstats.std = sqrt(sum_deviation/n);  
	rstats.min = min;
	rstats.max = max;
	return rstats;
    //return sqrt(sum_deviation/n);           
}