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

#define N 5
#define MAXCOUNT 5000000


#ifdef VERBOSE //debug info
#define VB(x) x
#else
#define VB(x) do { } while (0)
#endif


int flag = 0;
int counter =0;  // Common counter
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t threshold = PTHREAD_COND_INITIALIZER;

void *tf(void *param){
    int i;
	
    while (1) {
			pthread_mutex_lock(&mutex);
			
				
			if(counter == MAXCOUNT){
				//flag = 0;
				VB(printf("counter is reached, n = %d\n", counter));
				pthread_cond_signal(&threshold);
				pthread_mutex_unlock(&mutex);
				break;
				
			}
			
			counter++;
			
			pthread_mutex_unlock(&mutex);
	    }
    pthread_exit(NULL);
}

int main(void) {
    int i;
	flag = 1;
    pthread_t ti_arr[N];

	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&threshold, NULL);
	
	
    for (i = 0 ; i < N ; i++) {
        if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
            printf("Error in creating a thread %d\n", i);
            exit (0);
        }
    }
	VB(printf("Locking mutex\n"));
    pthread_mutex_lock(&mutex);
	VB(printf("Entering loop de loop\n"));
   
		pthread_cond_wait(&threshold, &mutex);
		VB(printf("Cond signal received, count is %d\n", counter));
	
		
	printf("Counter is after the while loop %d\n", counter);
	pthread_mutex_unlock(&mutex);
    for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
      }
	  
	

	printf("Counter is when all sub threads have finnished %d\n", counter);
            
    return 0;
}

