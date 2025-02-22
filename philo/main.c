/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:51 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 11:09:1 by aevstign         ###   ########.fr       */
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
	if (sim->philos)
		free(sim->philos);
	if (sim->forks)
		free(sim->forks);
	sim->philos = NULL;
	sim->forks = NULL;
}

static int	start_simulation(t_simulation *sim)
{
	int	i;

	if (!sim->forks || !sim->philos)
		return (-1);
	i = -1;
	sim->start_simulation = gettime_ms();
	while (++i < sim->philo_num)
		if (pthread_create(&sim->philos[i].thread_id,
				NULL, philosopher, &sim->philos[i]) != 0)
			return (handle_error(PTHREAD_CREATE_ERROR));
	set_int(&sim->sim_mutex, &sim->all_threads_ready, 1);
	if (sim->philo_num > 1)
		if (pthread_create(&sim->monitor, NULL, monitor, sim) != 0)
			return (handle_error(PTHREAD_CREATE_ERROR));
	return (SUCCESS);
}

static int	join_threads(t_simulation *sim)
{
	int	i;

	i = -1;
	while (++i < sim->philo_num)
	{
		if (pthread_join(sim->philos[i].thread_id, NULL) != 0)
			return (handle_error(PTHREAD_JOIN_ERROR));
	}
	set_int(&sim->sim_mutex, &sim->stop_flag, 1);
	if (sim->philo_num > 1)
		if (pthread_join(sim->monitor, NULL) != 0)
			return (handle_error(PTHREAD_JOIN_ERROR));
	clean(sim);
	return (SUCCESS);
}

int	handle_error(t_error_status status)
{
	if (status == MUTEX_INIT_ERROR)
		write(1, "philo error: failed to init mutex\n", 34);
	else if (status == MUTEX_LOCK_ERROR)
		write(1, "philo error: failed to lock mutex\n", 34);
	else if (status == MUTEX_UNLOCK_ERROR)
		write(1, "philo error: failed to unlock mutex\n", 36);
	else if (status == MUTEX_WRITE_ERROR)
		write(1, "philo error: failed to write with mutex\n", 40);
	return (status);
}

int	main(int argc, char **argv)
{
	t_simulation		sim = {0};
	
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
		if (join_threads(&sim) != SUCCESS)
		{
			clean(&sim);
			return (1);
		}
	}
	else
		ft_error("Error: wrong number of args");
}
