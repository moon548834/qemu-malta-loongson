#ifndef __TIME__H_
#define __TIME__H_ 1
struct timespec {
        long       ts_sec;
        long       ts_nsec;
};


struct tm {
	int sec;    /* Seconds (0-60) */
	int min;    /* Minutes (0-59) */
	int hour;   /* Hours (0-23) */
	int day;   /* Day of the month (1-31) */
	int mon;    /* Month (0-11) */
	int year;   /* Year - 1900 */
};


int gettimeofday(struct timespec *tv);
int localtime(long sec, struct tm *);
#endif
