


#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

#include "MultiSensorSimulator.h"




int main(void)
{
    
	int sensorDescriptors[10];
	StartSimulator(sensorDescriptors, 5); // Use 5 or 10
	// Generates 5 values per descriptor
	struct timespec current_time, lag, total_lag;
	//total_lag.tv_nsec = 0;
	//total_lag.tv_sec = 0;
	memset(&total_lag, 0, sizeof(struct timespec));
	fd_set fdset, exfdset;
	int n;
	int maxfd = 0;
	int flags[10] = { 0 };
	int nsensors_active = 10;
	Tmeas measurement;
	while(1){
		FD_ZERO(&fdset);
		for (int i = 0; i<10; i++) {
			if(flags[i] != -1)
				FD_SET(sensorDescriptors[i], &fdset);
			
			maxfd = ( sensorDescriptors[i] > maxfd ?  sensorDescriptors[i] : maxfd );
			//printf("descriptors: %d\n", sensorDescriptors[i]);
		}
		
		//printf("Max descriptor is %d\n", maxfd);
		n = select(maxfd+1, &fdset, NULL, NULL, NULL);
		
		if (n > 0) {
		
			for (int i = 0; i<10; i++) {
				if(FD_ISSET(sensorDescriptors[i], &fdset)){
					if(read(sensorDescriptors[i], &measurement, sizeof(Tmeas))) {
						clock_gettime(CLOCK_REALTIME, &current_time);
						lag = diff_timespec(&current_time, &measurement.moment );
						printf("# %d Measurement value of %d sensor is %d, it took %d\n", ++flags[i], i, measurement.value, lag.tv_nsec );
						increment_timespec(&total_lag, &lag);
						
					} else {
						nsensors_active--;
						flags[i] = -1;
						printf("-------------EOF in sensor #%d, %d remaining -------------\n", i, nsensors_active);
						
						
						
					}
				}
			}
		}
		
		//break;
		/*n = select(
		
		read(sensorDescriptors[0], &measurement, sizeof(Tmeas));
		sleep(1);
		printf("Measurement value was %d, it took %d\n", measurement.value, (unsigned)time(NULL) - measurement.moment.tv_sec );
	*/
	if(nsensors_active == 0)
		break;
	}
	printf("Total lag is %ld nanoseconds\n",  total_lag.tv_nsec);
    return 0;
}