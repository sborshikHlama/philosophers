#include "philo.h"

/*Spinlock*/
void	wait_all_threads(table_t *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
	usleep(1000);
}

/*Monitor waits untill all threads are running*/
bool	all_threads_running(pthread_mutex_t *mutex,
	long *threads, long philo_num)
{
	bool ret;

	ret = false;
	safe_mutex_handle(mutex, LOCK);
	if (*threads == philo_num)
		ret = true;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

void	increase_long(pthread_mutex_t *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK);
}

void	de_sync(t_philo *philo)
{
	if (philo->table->philo_num % 2 == 0)
	{
		if (philo->id == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}
