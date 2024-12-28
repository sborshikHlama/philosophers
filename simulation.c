/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:01:52 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/27 13:05:22 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*lone_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	precise_usleep(philo->sim->time_to_die);
	write_status(DIED, philo, DEBUG_MODE);
	pthread_mutex_unlock(&philo->first_fork->fork);
	return (NULL);
}

static	void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	pthread_mutex_lock(&philo->second_fork->fork);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal_time = gettime(MILISECOND);
	pthread_mutex_unlock(&philo->philo_mutex);
	precise_usleep(philo->sim->time_to_eat);
	philo->meals_counter++;
	write_status(EATING, philo, DEBUG_MODE);
	if (philo->meals_counter == philo->sim->meals_to_eat)
		set_int(&philo->philo_mutex, &philo->full, 1);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

static void	think(t_philo *philo, int silent)
{
	long	time_to_think;

	pthread_mutex_lock(&philo->philo_mutex);
	time_to_think = (philo->sim->time_to_die
			- (gettime(MILISECOND) - philo->last_meal_time)
			- philo->sim->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->philo_mutex);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent > 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == 0)
		write_status(THINKING, philo, DEBUG_MODE);
	precise_usleep(time_to_think);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->sim->sim_mutex);
	philo->sim->threads_running_num++;
	pthread_mutex_unlock(&philo->sim->sim_mutex);
	if (philo->sim->meals_to_eat == 0)
		return (NULL);
	wait_all_threads(philo->sim);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	if (philo->sim->philo_num == 1)
		return (lone_philo(philo));
	if (philo->id % 2)
		think(philo, 1);
	while (!simulation_finished(philo->sim))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->sim->time_to_sleep);
		think(philo, 0);
	}
	return (NULL);
}
