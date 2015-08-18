


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


int main(void)
{
    
	int sensorDescriptors[10];
	StartSimulator(sensorDescriptors, 5); // Use 5 or 10
	// Generates 5 values per descriptor
	struct timespec current_time, lag, total_lag;
	fd_set fdset, exfdset;
	int n;
	int maxfd = 0;
	int flags[10] = { 0 };
	int nsensors_active = 10;
	Tmeas measurement;
	
	
	struct sigaction sigact, oldact;
	struct aiocb aiocb; //can be local because rt signals are used
	
	sigemptyset(&sigact.sa_mask);
    sigact.sa_sigaction = char_ready_handler;
    sigact.sa_flags = SA_SIGINFO;
    if (sigaction(SIGIO, &sigact, &oldact) < 0) {
        fprintf(stderr, "\nCannot set signal handler");
        exit(0);
    } 
	
	aiocb.aio_buf = &measurement;
    aiocb.aio_nbytes = sizeof(Tmeas);
    aiocb.aio_offset = 0;
    aiocb.aio_reqprio = 0;
    aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL ;
    aiocb.aio_sigevent.sigev_signo = SIGIO ;
    aiocb.aio_sigevent.sigev_value.sival_ptr = &aiocb;
    aiocb.aio_lio_opcode = 0;
	
	for (int i = 0; i < 10; i++) {
		aiocb.aio_fildes = sensorDescriptors[i];
		aio_read(&aiocb);
	}
	
	
	
	sleep(45);
	
    return 0;
}





void char_ready_handler(int signo, siginfo_t* siginfo, void *dummy) {
    char answer;
	Tmeas *temp = (Tmeas*)(((struct aiocb*)siginfo->si_value.sival_ptr)->aio_buf);
    if(signo == SIGIO) {
       /* printf("i/o complete signal is received. Char was %d \n",
                 *((struct Tmeas*)(((struct aiocb*)siginfo->si_value.sival_ptr)->aio_buf)))->value;
        //start a new read*/
		
		 printf("i/o %d \n",
                 temp->value);
        aio_read(((struct aiocb*)siginfo->si_value.sival_ptr));
    }
}