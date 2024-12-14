/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:30:20 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 01:02:34 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_int(t_mutex *mutex, int *dest, int value)
{
	safe_mutex_op(mutex, LOCK);
	*dest = value;
	safe_mutex_op(mutex, UNLOCK);
}

int	get_int(t_mutex *mutex, int *value)
{
	int	ret;

	safe_mutex_op(mutex, LOCK);
	ret = *value;
	safe_mutex_op(mutex, UNLOCK);
	return (ret);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	safe_mutex_op(mutex, LOCK);
	*dest = value;
	safe_mutex_op(mutex, UNLOCK);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	safe_mutex_op(mutex, LOCK);
	ret = *value;
	safe_mutex_op(mutex, UNLOCK);
	return (ret);
}

int	simulation_finished(t_simulation *sim)
{
	return (get_int(&sim->sim_mutex, &sim->end_simulation));
}
