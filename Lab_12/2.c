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

#include <pthread.h>

#ifdef VERBOSE //debug info
#define VB(x) x
#else
#define VB(x) do { } while (0)
#endif

#define NINCREMENTS 10000000

void* tf(void*);
static long int counter = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void){
	pthread_t id[10];
	//long int *pstat[10];
	long int counters[10];
	int i;
	long long int accum = 0;
	VB(printf("Creating threads\n"));
	for(i = 0; i<10; i++){
		
		//pthread_create(&id[i], NULL, tf, NULL);
		pthread_create(&id[i], NULL, tf, (void*)&counters[i]);
		
	}
	sleep(1);
	VB(printf("Joining...\n"));
	for(i = 0; i<10; i++){
		VB(printf("Joining: thread %d\n", i));
		//pthread_join(id[i], (void**)(&pstat[i]/*+sizeof(long int)*i*/));
		pthread_join(id[i], NULL);
	}
	
	for(i=0;i<10;i++){
		VB(printf("Counter[%d]  is %d\n", i, counters[i]));
	}
	
	printf("Global counter is: %ld\n which is %ld less than it should be\n", counter, NINCREMENTS*10-counter);
	
	return 0;
}



void* tf(void* p) {
	long int lcounter = 0;
	int i;
	for(i = 0; i<NINCREMENTS; i++){
		pthread_mutex_lock(&mutex);
		lcounter++;
		counter++;
		pthread_mutex_unlock(&mutex);
		
	}

	//VB(printf("im a thread\n"));
	VB(printf("Counter counted %ld\n", lcounter));
	//status = 10;
	//pthread_exit((void*) &lcounter);
	*(int*)p = lcounter;
	VB(printf("returning from thread\n"));
	return NULL;


}