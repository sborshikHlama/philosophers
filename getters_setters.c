#include "philo.h"

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

bool	get_bool(pthread_mutex_t  *mutex, bool *value)
{
	safe_mutex_handle(mutex, LOCK);
	return (value);
	safe_mutex_handle(mutex, UNLOCK);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	return (value);
	safe_mutex_handle(mutex, UNLOCK);
}

bool	simulation_finished(table_t *table)
{
	return get_bool(&table->table_mutex, &table->end_simulation);
}
