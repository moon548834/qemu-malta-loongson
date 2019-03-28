#include <time.h>
#include <cmdline.h>
#include "irq.h"


#define MYHZ 1

static int cp0timer_irq_handler(int irq, void *data)
{
	static int cnt=0;
	printf("cnt %d\n",cnt++);
	write_c0_compare(read_c0_count()+CPUFREQ/2/MYHZ);
	return IRQ_HANDLED;
}

int __attribute__ ((constructor)) timer_init()
{
	printf("time init\n");
	request_irq(CPU_IRQ_BASE+7, cp0timer_irq_handler, 0, "timer" , (void *)0x1234);
	write_c0_compare(read_c0_count()+CPUFREQ/2/MYHZ);

	return 0;
}
