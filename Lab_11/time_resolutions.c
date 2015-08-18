/* time_resolutions.c -- This program tries to find out time resolutions
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

// Functions for struct timespec and nanoseconds
long long int getnanosec(const struct timespec* ts);
struct timespec maketimespec(long long int ns);
struct timespec timespecdiff(const struct timespec* ts1, 
			const struct timespec* ts2);
void timespecdisplay(const struct timespec* ts);
int timespec_greater_than(const struct timespec* ts1, 
			const struct timespec* ts2);

int main(void) {
    int n;
    long int clock_ticks_per_seconds;
  
    struct timespec real_time1, real_time2, real_time_diff, real_time_res;
    struct timespec start, now, nano_sleep;

    struct itimerspec timer_struct, start_timerspec, now_timerspec;
    timer_t timer_id;
    struct sigevent sigevent;

    clock_t clock_start, clock_now, clock_diff;
  
  
    clock_ticks_per_seconds = sysconf(_SC_CLK_TCK);
    printf("Clock ticks per second is according sysconf :%ld\n",
			 clock_ticks_per_seconds);
    clock_getres(CLOCK_REALTIME, &real_time_res);
    printf("Real time clock resolution is  :%ld s %ld ns\n", 
		real_time_res.tv_sec, real_time_res.tv_nsec );
  
    // Own test for clock resolution
    clock_gettime(CLOCK_REALTIME, &start);
    n = 0;
    do {
        clock_gettime(CLOCK_REALTIME, &now);
        n++;
    } while(!timespec_greater_than(&now,&start)); // !(now > start)
    printf("Tested real time clock resolution is ");
    real_time_diff = timespecdiff(&now, &start);  
    timespecdisplay(&real_time_diff);
    printf("Clock has been read %d times\n", n );
  
    // Interval timer without signals (POSIX:TMR)
    // Interval timer is used for time measurements  
    sigevent.sigev_notify = SIGEV_NONE;
    timer_create(CLOCK_REALTIME, &sigevent, &timer_id);

    timer_struct.it_value.tv_sec = 100;
    timer_struct.it_value.tv_nsec = 0;
    timer_struct.it_interval.tv_sec = 0;
    timer_struct.it_interval.tv_nsec = 0;
    timer_settime(timer_id, 0 , &timer_struct, NULL);
  
    timer_gettime(timer_id, &start_timerspec);
    n = 0;
    do {
        timer_gettime(timer_id, &now_timerspec);
        n++;
    } while(!timespec_greater_than(&start_timerspec.it_value,
		&now_timerspec.it_value)); // !(start > now)
    printf("Tested timer clock resolution is ");
    real_time_diff = timespecdiff(&start_timerspec.it_value,
		 &now_timerspec.it_value);
    timespecdisplay(&real_time_diff);
    printf("Clock has been read %d times\n\n", n );
  
  
    // Conventional clock times
    clock_start = times(NULL);;
    n = 0;
    do {
        clock_now = times(NULL);
        n++;
    } while(!(clock_now > clock_start)); 
    clock_diff = clock_now - clock_start;  
    printf("Tested times function resolution is  %d ticks\n", clock_diff);
    printf("Clock has been read %d times\n", n );
 
// Some measurements
    //nano_sleep.tv_sec = 0;   
    nano_sleep.tv_nsec = 100;     // 100 ns   ->     // 250000 ns - 350000ns
    //nano_sleep.tv_nsec = 1000;    // 1 us     ->     // 250000 ns - 350000ns
    //nano_sleep.tv_nsec = 10000;   // 10 us    ->     // 250000 ns - 350000ns
    //nano_sleep.tv_nsec = 100000;  // 100 us          // 250000 ns - 350000ns
    //nano_sleep.tv_nsec = 1000000; // 1000 us eli 1 ms  // 1.11 ms - 1.25 ms    
    //nano_sleep.tv_nsec = 10000000;// 10 ms             //10.11 ms - 10.22 ms
    nano_sleep.tv_nsec = 100000000; // 100 ms            //100.11 ms - 100.55 ms
    
    //Real time clock
    printf("\n");
    clock_gettime(CLOCK_REALTIME, &real_time1);
    //printf("Real time clock now is: ");
    //timespecdisplay(&real_time1);
    nanosleep(&nano_sleep, NULL);
    clock_gettime(CLOCK_REALTIME, &real_time2);
    //printf("Real time clock later is: ");
    //timespecdisplay(&real_time2);
    real_time_diff = timespecdiff(&real_time2, &real_time1);
    printf("Time difference is: ");
    timespecdisplay(&real_time_diff);
    printf("\n"); 
    return 0;
}


// Functions for struct timespec and nanoseconds
long long int getnanosec(const struct timespec* ts) {
    return ((long long int)ts->tv_sec)*1000000000 + ts->tv_nsec;
};

struct timespec maketimespec(long long int ns) {
    struct timespec aux;
    aux.tv_sec = ns/1000000000;
    aux.tv_nsec = ns%1000000000;
    return aux;
};

struct timespec timespecdiff(const struct timespec* ts1,
			const struct timespec* ts2) {
    long long int time1_nanos = getnanosec(ts1);
    long long int time2_nanos = getnanosec(ts2);
    long long int timediff_nanos = time1_nanos - time2_nanos;
    return maketimespec(timediff_nanos);
}
void timespecdisplay(const struct timespec* ts) {
    printf("%ld s %ld ns\n", ts->tv_sec, ts->tv_nsec);
}
int timespec_greater_than(const struct timespec* ts1, 
			const struct timespec* ts2) {
    long long int time1_nanos = getnanosec(ts1);
    long long int time2_nanos = getnanosec(ts2);
    return time1_nanos > time2_nanos;
}
