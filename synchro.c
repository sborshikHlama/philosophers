#include "philo.h"

/*Spinlock*/
void	wait_all_threads(table_t *table)
{
	while (get_bool(&table->table_mutex, &table->all_threads_ready))
	;
}

