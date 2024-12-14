/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:28:11 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 17:00:15 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_simulation *sim)
{
	while (!get_int(&sim->sim_mutex, &sim->all_threads_ready))
		usleep(1000);
}

int	all_threads_running(t_mutex *mutex,
	long *threads, long philo_num)
{
	int	ret;

	ret = 0;
	safe_mutex_op(mutex, LOCK);
	if (*threads == philo_num)
		ret = 1;
	safe_mutex_op(mutex, UNLOCK);
	return (ret);
}

void	increase_long(t_mutex *mutex, long *value)
{
	safe_mutex_op(mutex, LOCK);
	(*value)++;
	safe_mutex_op(mutex, UNLOCK);
}
