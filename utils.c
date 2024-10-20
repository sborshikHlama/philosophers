#include "philo.h"

/*Create timer*/
long	gettime(t_time_code time_code)
{
	/*Define time value*/
	struct timeval	tv;
	/*takes time value pointer and time zone pointer*/
	/*returns 0 if success and -1 if fail*/
	if (gettimeofday(&tv, NULL) < 0)
		error_exit("Error: gettimeofday failed");
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_sec /  1e6));
	else if (time_code == MILISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		error_exit("Error: wrong input to gettimeofday");
	return (52);
}

void precise_usleep(long usec, table_t *table)
{
	long	start;
	long	elapsed;
	long	rem;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < usec)
	{
		usleep(500);	
	}
}

void	error_exit(const char	*msg)
{
	ft_putendl_fd((char *)msg, 1);
	exit(1);
}


