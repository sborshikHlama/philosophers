/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:28:11 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 11:06:12 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_simulation *sim)
{
	while (get_int(&sim->sim_mutex,
			&sim->threads_running_num) != sim->philo_num)
		usleep(300);
}

void	increase_long(t_mutex *mutex, long *value)
{
	pthread_mutex_lock(mutex);
	(*value)++;
	pthread_mutex_unlock(mutex);
}

int	increase_int(t_mutex *mutex, int *num)
{
	if (pthread_mutex_lock(mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	(*num)++;
	if (pthread_mutex_unlock(mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}
