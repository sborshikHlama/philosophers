/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:01:52 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/19 18:28:51 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lone_philo(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->first_fork->fork) != 0)
		return (MUTEX_LOCK_ERROR);
	if (write_status(TAKE_FIRST_FORK, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	precise_usleep(philo->sim->time_to_die);
	if (write_status(DIED, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	if (pthread_mutex_unlock(&philo->first_fork->fork) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}

void	*handle_error(t_error_status status)
{
	if (status == MUTEX_INIT_ERROR)
		write(1, "philo error: failed to init mutex\n", 34);
	else if (status == MUTEX_LOCK_ERROR)
		write(1, "philo error: failed to lock mutex\n", 34);
	else if (status == MUTEX_UNLOCK_ERROR)
		write(1, "philo error: failed to unlock mutex\n", 36);
	else if (status == MUTEX_WRITE_ERROR)
		write(1, "philo error: failed to write with mutex\n", 40);
	return (NULL);
}

static	int	eat(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->first_fork->fork) != 0)
		return (MUTEX_LOCK_ERROR);
	write_status(TAKE_FIRST_FORK, philo);
	if (pthread_mutex_lock(&philo->second_fork->fork) != 0)
		return (MUTEX_LOCK_ERROR);
	write_status(TAKE_SECOND_FORK, philo);
	if (pthread_mutex_lock(&philo->philo_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	philo->last_meal_time = gettime(MILISECOND);
	if (pthread_mutex_unlock(&philo->philo_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	if (write_status(EATING, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	increase_long(&philo->philo_mutex, &philo->meals_counter);
	precise_usleep(philo->sim->time_to_eat);
	if (philo->meals_counter == philo->sim->meals_to_eat)
		set_int(&philo->philo_mutex, &philo->full, 1);
	if (pthread_mutex_unlock(&philo->first_fork->fork) != 0)
		return (MUTEX_UNLOCK_ERROR);
	if (pthread_mutex_unlock(&philo->second_fork->fork) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}

static int	think(t_philo *philo, int silent)
{
	long	time_to_think;

	if (pthread_mutex_lock(&philo->philo_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	time_to_think = (philo->sim->time_to_die
			- (gettime(MILISECOND) - philo->last_meal_time)
			- philo->sim->time_to_eat) / 2;
	if (pthread_mutex_unlock(&philo->philo_mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent > 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == 0)
		if (write_status(THINKING, philo) == MUTEX_WRITE_ERROR)
			return (MUTEX_WRITE_ERROR);
	precise_usleep(time_to_think);
	return (SUCCESS);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	handle_error(increase_int(&philo->sim->sim_mutex,
			&philo->sim->threads_running_num));
	if (philo->sim->meals_to_eat == 0)
		return (NULL);
	wait_all_threads(philo->sim);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILISECOND));
	if (philo->sim->philo_num == 1)
		return (handle_error(lone_philo(philo)));
	if (philo->id % 2)
		handle_error(think(philo, 1));
	while (!simulation_finished(philo->sim))
	{
		if (philo->full)
			break ;
		handle_error(eat(philo));
		handle_error(write_status(SLEEPING, philo));
		precise_usleep(philo->sim->time_to_sleep);
		handle_error(think(philo, 0));
	}
	return (NULL);
}
