/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:03:27 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 03:12:20 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		error_exit("Error: gettimeofday failed");
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_sec / 1e6));
	else if (time_code == MILISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		error_exit("Error: wrong input to gettimeofday");
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

void	error_exit(const char	*msg)
{
	printf("%s\n", msg);
	exit(1);
}
