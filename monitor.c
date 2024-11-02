#include "philo.h"

static bool	is_philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;
	long	current_time;
	long	last_meal_time;
	/*if philo is not dead but just full*/
	if (get_bool(&philo->table->table_mutex, &philo->full))
		return (false);
	current_time = gettime(MILISECOND);
	last_meal_time = get_long(&philo->philo_mutex, &philo->last_meal_time);
	elapsed = current_time - last_meal_time;
	time_to_die = philo->table->time_to_die / 1e3;
	/*if philo is dead*/
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	table_t	*table;
	table = (table_t *)data;
	while (!all_threads_running(&table->table_mutex,
		&table->threads_running_num, table->philo_num))
		usleep(1000);
	while (!simulation_finished(table))
	{
		int	i;

		i = 0;
		while (i < table->philo_num && !simulation_finished(table))
		{
			if (is_philo_died(table->philos + i))
			{
				write_status(DIED, table->philos + i, DEBUG_MODE);
				set_bool(&table->table_mutex, &table->end_simulation, true);
			}
			i++;
		}
	}
	return (NULL);
}