/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:51 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/11 15:27:17 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_simulation *sim)
{
	int		i;
	t_philo	*philo;
	t_fork	*fork;

	i = 0;
	while (i < sim->philo_num)
	{
		philo = sim->philos + i;
		fork = sim->forks + i;
		pthread_mutex_destroy(&philo->philo_mutex);
		pthread_mutex_destroy(&fork->fork);
		i++;
	}
	pthread_mutex_destroy(&sim->write_mutex);
	pthread_mutex_destroy(&sim->sim_mutex);
	free(sim->philos);
	free(sim->forks);
	sim->philos = NULL;
	sim->forks = NULL;
}

static int	start_simulation(t_simulation *sim)
{
	int	i;

	i = -1;
	sim->start_simulation = gettime(MILISECOND);
	while (++i < sim->philo_num)
		if (pthread_create(&sim->philos[i].thread_id,
				NULL, philosopher, &sim->philos[i]) != 0)
			return (PTHREAD_CREATE_ERROR);
	set_int(&sim->sim_mutex, &sim->all_threads_ready, 1);
	if (sim->philo_num > 1)
		if (pthread_create(&sim->monitor, NULL, monitor, sim) != 0)
			return (PTHREAD_CREATE_ERROR);
	return (SUCCESS);
}

static int	join_threads(t_simulation *sim)
{
	int	i;

	i = -1;
	while (++i < sim->philo_num)
		pthread_join(sim->philos[i].thread_id, NULL);
	set_int(&sim->sim_mutex, &sim->stop_flag, 1);
	if (sim->philo_num > 1)
		pthread_join(sim->monitor, NULL);
	clean(sim);
	return (1);
}

int	inc_running_threads(t_mutex *mutex, int num)
{
	if (pthread_mutex_lock(mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	num++;
	if (pthread_mutex_unlock(mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_simulation		sim;

	if (argc == 5 || argc == 6)
	{
		if (parse_input(&sim, argv) != SUCCESS)
			return (1);
		if (init(&sim) != SUCCESS)
		{
			clean(&sim);
			return (1);
		}
		if (start_simulation(&sim) != SUCCESS)
		{
			clean(&sim);
			return (1);
		}
		join_threads(&sim);
	}
	else
		ft_error("Error: wrong number of args");
}
