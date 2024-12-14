/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:01:52 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 17:13:44 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*lone_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->sim);
	safe_mutex_op(&philo->sim->sim_mutex, LOCK);
	philo->sim->threads_running_num++;
	safe_mutex_op(&philo->sim->sim_mutex, UNLOCK);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->sim))
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
	precise_usleep(philo->sim->time_to_eat);
	if (philo->sim->meals_to_eat > 0
		&& philo->meals_counter == philo->sim->meals_to_eat)
		set_int(&philo->philo_mutex, &philo->full, 1);
	safe_mutex_op(&philo->first_fork->fork, UNLOCK);
	safe_mutex_op(&philo->second_fork->fork, UNLOCK);
}

static void	think(t_philo *philo)
{
	write_status(THINKING, philo, DEBUG_MODE);
	if (philo->sim->philo_num % 2)
		precise_usleep(philo->sim->time_to_eat / 2);
}

static void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->sim);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	increase_long(&philo->sim->sim_mutex,
		&philo->sim->threads_running_num);
	while (!simulation_finished(philo->sim))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->sim->time_to_sleep);
		think(philo);
	}
	return (NULL);
}

void	simulate(t_simulation	*sim)
{
	int	i;

	i = -1;
	if (sim->meals_to_eat == 0)
		return ;
	else if (sim->philo_num == 1)
		safe_thread_op(&sim->philos[0].thread_id,
			lone_philo, &sim->philos[0], CREATE);
	else
		while (++i < sim->philo_num)
			safe_thread_op(&sim->philos[i].thread_id, philosopher,
				&sim->philos[i], CREATE);
	safe_thread_op(&sim->monitor, monitor, sim, CREATE);
	sim->start_simulation = gettime(MILISECOND);
	set_int(&sim->sim_mutex, &sim->all_threads_ready, 1);
	i = -1;
	while (++i < sim->philo_num)
		safe_thread_op(&sim->philos[i].thread_id, NULL, NULL, JOIN);
	set_int(&sim->sim_mutex, &sim->end_simulation, 1);
	safe_thread_op(&sim->monitor, NULL, NULL, JOIN);
}
