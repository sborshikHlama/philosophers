/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:01:52 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/24 09:12:26 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lone_philo(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->first_fork->fork) != 0)
		return (MUTEX_LOCK_ERROR);
	if (write_status(TAKE_FIRST_FORK, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	precise_usleep(philo->sim, philo->sim->time_to_die);
	if (write_status(DIED, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	if (pthread_mutex_unlock(&philo->first_fork->fork) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}

void	*wrapper(int status, t_simulation *sim)
{
	handle_error(status);
	if (status != SUCCESS)
		set_int(&sim->sim_mutex, &sim->error_flag, 1);
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
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime_ms());
	if (write_status(EATING, philo) == MUTEX_WRITE_ERROR)
		return (MUTEX_WRITE_ERROR);
	precise_usleep(philo->sim, philo->sim->time_to_eat);
	increase_int(&philo->philo_mutex, &philo->meals_counter);
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
	time_t	time_to_think;
	time_t	time_from_last_meal;
	time_t	time_till_death;

	if (pthread_mutex_lock(&philo->philo_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	time_from_last_meal = gettime_ms() - philo->last_meal_time;
	time_till_death = (philo->sim->time_to_die) - time_from_last_meal;
	time_to_think = (time_till_death
			- (philo->sim->time_to_eat)) / 2;
	if (pthread_mutex_unlock(&philo->philo_mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == 0)
		if (write_status(THINKING, philo) == MUTEX_WRITE_ERROR)
			return (MUTEX_WRITE_ERROR);
	precise_usleep(philo->sim, time_to_think);
	return (SUCCESS);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->sim->meals_to_eat == 0)
		return (NULL);
	wrapper(increase_int(&philo->sim->sim_mutex,
			&philo->sim->threads_running_num), philo->sim);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime_ms());
	wait_all_threads(philo->sim);
	if (philo->sim->philo_num == 1)
		return (wrapper(lone_philo(philo), philo->sim));
	else if (philo->id % 2)
		wrapper(think(philo, 1), philo->sim);
	while (!simulation_finished(philo->sim))
	{
		if (get_int(&philo->philo_mutex, &philo->full))
			break ;
		pthread_mutex_unlock(&philo->philo_mutex);
		wrapper(eat(philo), philo->sim);
		wrapper(write_status(SLEEPING, philo), philo->sim);
		precise_usleep(philo->sim, philo->sim->time_to_sleep);
		wrapper(think(philo, 0), philo->sim);
	}
	return (NULL);
}
