/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:01:52 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/08 21:58:28 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*lone_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	safe_mutex_op(&philo->table->table_mutex, LOCK);
	philo->table->threads_running_num++;
	safe_mutex_op(&philo->table->table_mutex, UNLOCK);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->table))
		usleep(200);
	return (NULL);
}

static	void	eat(t_philo *philo)
{
	safe_mutex_op(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	safe_mutex_op(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	philo->meals_counter++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat);
	if (philo->table->limit_meals > 0
		&& philo->meals_counter == philo->table->limit_meals)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex_op(&philo->first_fork->fork, UNLOCK);
	safe_mutex_op(&philo->second_fork->fork, UNLOCK);
}

void	think(t_philo *philo, bool pre_simulation)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (pre_simulation)
		write_status(THINKING, philo, DEBUG_MODE);
	if (philo->table->philo_num % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(t_think * 0.52);
}

static void	*simulate(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_num);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->table->time_to_sleep);
		think(philo, false);
	}
	return (NULL);
}

void	run_threads(t_table	*table)
{
	int	i;

	i = -1;
	if (table->limit_meals == 0)
		return ;
	else if (table->philo_num == 1)
		safe_thread_op(&table->philos[0].thread_id,
			lone_philo, &table->philos[0], CREATE);
	else
		while (++i < table->philo_num)
			safe_thread_op(&table->philos[i].thread_id, simulate,
				&table->philos[i], CREATE);
	safe_thread_op(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simulation = gettime(MILISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_num)
		safe_thread_op(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_op(&table->monitor, NULL, NULL, JOIN);
}
