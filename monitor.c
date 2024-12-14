/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:28:44 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 17:10:05 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;
	long	current_time;
	long	last_meal_time;

	if (get_int(&philo->sim->sim_mutex, &philo->full))
		return (0);
	current_time = gettime(MILISECOND);
	last_meal_time = get_long(&philo->philo_mutex, &philo->last_meal_time);
	elapsed = current_time - last_meal_time;
	time_to_die = philo->sim->time_to_die / 1e3;
	if (elapsed > time_to_die)
		return (1);
	return (0);
}

void	*monitor(void *data)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)data;
	while (!all_threads_running(&sim->sim_mutex,
			&sim->threads_running_num, sim->philo_num))
		usleep(1000);
	while (!simulation_finished(sim))
	{
		i = 0;
		while (i < sim->philo_num && !simulation_finished(sim))
		{
			if (is_philo_died(sim->philos + i))
			{
				write_status(DIED, sim->philos + i, DEBUG_MODE);
				set_int(&sim->sim_mutex, &sim->end_simulation, 1);
			}
			i++;
		}
	}
	return (NULL);
}
