/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:30:20 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/16 11:15:22 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_int(t_mutex *mutex, int *dest, int value)
{
	pthread_mutex_lock(mutex);
	*dest = value;
	pthread_mutex_unlock(mutex);
}

int	get_int(t_mutex *mutex, int *value)
{
	int	ret;

	pthread_mutex_lock(mutex);
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	pthread_mutex_lock(mutex);
	*dest = value;
	pthread_mutex_unlock(mutex);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	pthread_mutex_lock(mutex);
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

int	simulation_finished(t_simulation *sim)
{
	return (get_int(&sim->sim_mutex, &sim->stop_flag));
}
