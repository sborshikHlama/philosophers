/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:03:27 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/21 23:40:36 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	gettime_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(t_simulation *sim, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = gettime_ms() + sleep_time;
	while (gettime_ms() < wake_up)
	{
		if (simulation_finished(sim))
			break ;
		usleep(100);
	}
}

int	ft_error(const char	*msg)
{
	printf("%s\n", msg);
	return (1);
}

void	finish_simulation(t_simulation *sim)
{
	set_int(&sim->sim_mutex, &sim->stop_flag, 1);
}

void	sim_delay(t_simulation *sim)
{
	while (gettime_ms() < sim->start_simulation)
		continue ;
}
