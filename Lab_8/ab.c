


#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <aio.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

#include "MultiSensorSimulator.h"


void char_ready_handler(int signo, siginfo_t* siginfo, void *dummy);
struct aiocb aiocb[10]; //can be local because rt signals are used
struct timespec current_time, lag, total_lag;
int NSENS = 10;
int main(void)
{
    
	
	

	
	
	
	int sensorDescriptors[10];
	StartSimulator(sensorDescriptors, 5); // Use 5 or 10
	// Generates 5 values per descriptor

	memset(&total_lag, 0, sizeof(struct timespec));
	
	
	fd_set fdset, exfdset;
	int n;
	int maxfd = 0;
	int flags[10] = { 0 };
	int nsensors_active = 10;
	Tmeas measurement;
	
	
	struct sigaction sigact, oldact;
	
	
	sigemptyset(&sigact.sa_mask);
    sigact.sa_sigaction = char_ready_handler;
    sigact.sa_flags = SA_SIGINFO;
    if (sigaction(SIGIO, &sigact, &oldact) < 0) {
        fprintf(stderr, "\nCannot set signal handler");
        exit(0);
    } 
	

	
	for (int i = 0; i < 10; i++) {
		aiocb[i].aio_buf = &measurement;
		aiocb[i].aio_nbytes = sizeof(Tmeas);
		aiocb[i].aio_offset = 0;
		aiocb[i].aio_reqprio = 0;
		aiocb[i].aio_sigevent.sigev_notify = SIGEV_SIGNAL ;
		aiocb[i].aio_sigevent.sigev_signo = SIGIO ;
		aiocb[i].aio_sigevent.sigev_value.sival_ptr = &aiocb[i];
		aiocb[i].aio_lio_opcode = 0;
		
		
		aiocb[i].aio_fildes = sensorDescriptors[i];
		aio_read(&aiocb[i]);
	}
	
	
	
	//sleep(45);
	//sleep(45);	
	//for(int i = 0; i<50; i++)
	//	sleep(5);
	while(NSENS);
	printf("All is done, the total lag is %d nanosecs\n", total_lag.tv_nsec);
    return 0;
}





void char_ready_handler(int signo, siginfo_t* siginfo, void *dummy) {
    char answer;
	Tmeas *temp = (Tmeas*)(((struct aiocb*)siginfo->si_value.sival_ptr)->aio_buf);
    
	int ptemp = ((struct aiocb*)siginfo->si_value.sival_ptr) - &aiocb[0];
	int intemp = (int)(((struct aiocb*)siginfo->si_value.sival_ptr)->aio_fildes);
	if (aio_error(((struct aiocb*)siginfo->si_value.sival_ptr)) != 0) 
		printf("ERROR\n");
	
	if( signo == SIGIO ) {
       /* printf("i/o complete signal is received. Char was %d \n",
                 *((struct Tmeas*)(((struct aiocb*)siginfo->si_value.sival_ptr)->aio_buf)))->value;
        //start a new read*/
		

			

		clock_gettime(CLOCK_REALTIME, &current_time);
		lag = diff_timespec(&current_time, &temp->moment );

		 printf("# Measurement value of %d sensor is %d, it took %d\n",
                  ptemp+1, temp->value, lag.tv_nsec);

		increment_timespec(&total_lag, &lag);
			
				 
		if (aio_return(((struct aiocb*)siginfo->si_value.sival_ptr)) > 0 )		 
			aio_read(((struct aiocb*)siginfo->si_value.sival_ptr));
		else
			printf("-------------EOF in sensor #%d, %d remaining--------------------\n", ptemp+1, --NSENS);
    }
}