/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:03:27 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/16 15:48:49 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		ft_error("Error: gettimeofday failed");
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_sec / 1e6));
	else if (time_code == MILISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
	{
		ft_error("Error: wrong input to gettimeofday");
	}
	return (52);
}

void	precise_usleep(long usec)
{
	long	start;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < usec)
	{
		usleep(500);
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
	while (gettime(MILISECOND) < sim->start_simulation)
		continue ;
}
