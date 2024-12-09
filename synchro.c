/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:28:11 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/07 16:22:04 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		usleep(1000);
}

bool	all_threads_running(t_mutex *mutex,
	long *threads, long philo_num)
{
	bool	ret;

	ret = false;
	safe_mutex_op(mutex, LOCK);
	if (*threads == philo_num)
		ret = true;
	safe_mutex_op(mutex, UNLOCK);
	return (ret);
}

void	increase_long(t_mutex *mutex, long *value)
{
	safe_mutex_op(mutex, LOCK);
	(*value)++;
	safe_mutex_op(mutex, UNLOCK);
}

void	de_sync(t_philo *philo)
{
	if (philo->table->philo_num % 2 == 0)
	{
		if (philo->id == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}
