#include "philo.h"
#include "stdio.h"

//TO DO start simualtion

static void assign_fork(t_philo *philo, t_fork *forks, int position)
{
	int	philo_num;

	philo_num = philo->table->philo_num;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[position];
		philo->second_fork = &forks[(position + 1) % philo_num];
	}
	else
	{
		philo->first_fork = &forks[(position + 1) % philo_num];
		philo->second_fork = &forks[position];
	}
}

static void philo_init(table_t *table)
{
	long	i;
	t_philo	*philo;
	i = 0;
	while (i < table->philo_num)
	{
		philo = table->philos + i;
		philo->meals_counter = 0;
		philo->table = table;
		philo->full = false;
		philo->id = i + 1;
		safe_mutex_handle(&philo->philo_mutex, INIT);
		assign_fork(philo, table->forks, i);
		i++;
	}
}

void	data_init(table_t	*table)
{
	int	i;

	i = 0;	
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running_num = 0;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_num);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_num);
	safe_mutex_handle(&table->table_mutex, INIT);
	safe_mutex_handle(&table->write_mutex, INIT);
	/*This is interesting part of memory use*/
	while (i < table->philo_num)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].id = i;
		i++;
	}
	philo_init(table);
}
