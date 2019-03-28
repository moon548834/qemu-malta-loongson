#include <time.h>
#include <cmdline.h>
int idle()
{
	static int count;
	int t = read_c0_count();
	if(((t-count)>(CPUFREQ/2/MYHZ)))
	{
		count = t;
	 printf("sr=0x%x count=0x%x compare=0x%x\n", read_c0_status(), read_c0_count(), read_c0_compare());
	}
	return 0;
}

