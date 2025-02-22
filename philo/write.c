/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:40:49 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 00:21:13 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// write [time_ms] [philo_id] [action]
int	write_status(t_philo_status status, t_philo *philo)
{
	time_t	elapsed;

	elapsed = gettime_ms() - philo->sim->start_simulation;
	if (philo->full)
		return (1);
	if (pthread_mutex_lock(&philo->sim->write_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	else
	{
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !simulation_finished(philo->sim))
			printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
		else if (status == EATING && !simulation_finished(philo->sim))
			printf(GREEN"%-6ld %d is eating\n"RESET, elapsed, philo->id);
		else if (status == SLEEPING && !simulation_finished(philo->sim))
			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
		else if (status == THINKING && !simulation_finished(philo->sim))
			printf("%-6ld %d is thinking\n", elapsed, philo->id);
		else if (status == DIED && !simulation_finished(philo->sim))
			printf(RED"%-6ld %d  died\n"RESET, elapsed, philo->id);
	}
	if (pthread_mutex_unlock(&philo->sim->write_mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}
