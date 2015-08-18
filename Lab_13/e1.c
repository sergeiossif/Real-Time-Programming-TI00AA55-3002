// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program. 
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly 
//    MAXCOUNT.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define N 2
#define MAXCOUNT 5


#ifdef VERBOSE //debug info
#define VB(x) x
#else
#define VB(x) do { } while (0)
#endif


int flag = 0;
int ready = 0;
int counter =0;  // Common counter
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

void *tf(void *param){
    int i = 0;
	
	pthread_mutex_lock(&mutex1);
	ready = 1;
		
	for(i=0;i<MAXCOUNT; i++){
		
		//
		pthread_cond_wait(&cond1, &mutex1);
		VB(printf("2: Im in!\n"));
		if(flag == 1){
			printf("2");
			fflush(0);
			flag = 0;
		} 
		pthread_cond_signal(&cond1);
	
	
	
	}
	pthread_cond_signal(&cond1);
	printf("Achivement unlocked\n");
    pthread_exit(NULL);
}

int main(void) {
    int i;
	flag = 1;
    pthread_t ti_arr;

	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_cond_init (&cond1, NULL);
	pthread_cond_init (&cond2, NULL);
	
	
	pthread_create(&ti_arr, NULL, tf, NULL);
 

 
	//pthread_cond_wait(&cond1, &mutex1);
	//VB(printf("callin cond2\n"));
	//pthread_cond_signal(&cond2);

	flag = 1;
	VB(printf("Sengind cond1\n"));
	//sleep(1);
	while(ready!=1){
		//printf("ready is not ready\n");
		
		}
		pthread_cond_signal(&cond1);
		
	for(i=0;i<MAXCOUNT; i++){
		
		pthread_cond_wait(&cond1, &mutex1);
		VB(printf("1: Im in!\n"));
		if(flag == 0){
			printf("1");
			fflush(0);
			flag = 1;
		} 
		pthread_cond_signal(&cond1);
	
	}
		
		printf("done!\n");
     return 0;
}

