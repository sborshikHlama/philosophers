/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:28:44 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 11:52:42 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_philo_died(t_philo *philo)
{
	time_t	elapsed;
	time_t	time_to_die;
	time_t	current_time;
	time_t	last_meal_time;

	if (get_int(&philo->philo_mutex, &philo->full))
		return (0);
	current_time = gettime_ms();
	last_meal_time = get_long(&philo->philo_mutex, &philo->last_meal_time);
	elapsed = current_time - last_meal_time;
	time_to_die = philo->sim->time_to_die;
	if (elapsed > time_to_die)
		return (1);
	return (0);
}

void	*monitor(void *data)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)data;
	wait_all_threads(sim);
	usleep(1000);
	while (!simulation_finished(sim))
	{
		pthread_mutex_lock(&sim->sim_mutex);
		if (sim->error_flag == 1)
			sim->stop_flag = 1;
		pthread_mutex_unlock(&sim->sim_mutex);
		i = 0;
		while (i < sim->philo_num && !simulation_finished(sim))
		{
			if (is_philo_died(sim->philos + i))
			{
				write_status(DIED, sim->philos + i);
				set_int(&sim->sim_mutex, &sim->stop_flag, 1);
			}
			i++;
		}
	}
	return (NULL);
}
