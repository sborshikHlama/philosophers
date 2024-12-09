/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:30:20 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/07 13:44:31 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	safe_mutex_op(mutex, LOCK);
	*dest = value;
	safe_mutex_op(mutex, UNLOCK);
}

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

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

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}
