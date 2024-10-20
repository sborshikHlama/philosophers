#include "philo.h"

static	void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->fork, LOCK);
	write_status(EATING, philo, DEBUG_MODE);
	safe_mutex_handle(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);

	set_long(&philo->philo_mutex, &philo->last_meal, gettime(MILISECOND));
	philo->meals_counter++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat, philo->table);
	/*Check if philosopher is full*/
	if (philo->table->limit_meals > 0
		&& philo->meals_counter == philo->table->limit_meals > 0)
		set_bool(&philo->philo_mutex, &philo->full, true);
	
	safe_mutex_handle(&philo->first_fork->fork, UNLOCK);
	safe_mutex_handle(&philo->second_fork->fork, UNLOCK);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	/*Synclock*/
	wait_all_threads(philo->table);

	/*Last time of meal*/

	while (!simulation_finished(philo->table->end_simulation))
	{
		/*Check if full*/
		if (philo->full)
			break ;
		/*Eat*/
		eat(philo);
		/*Sleep*/
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		/*Think*/
		think(philo);
	}

	return (NULL);
}

void	dinner_init(table_t	*table)
{	
	int	i;

	i = 0;
	if (table->limit_meals == 0)
		return ;
	else if (table->philo_num == 1)
		/*TODO*/;
	else
	/*Create all threads*/
		while (i++ < table->philo_num)
			safe_thread_handle(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	/*Create a timer for simulation */
	table->start_simulation = gettime(MILISECOND);
	/*Set state inside struct to true when all threads are ready*/
	set_bool(&table->table_mutex, &table->all_threads_ready, true);

	/*Wait for everyone*/
	i  = 0;
	while (i++ < table->philo_num)
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);

	/*All philos are full*/
}