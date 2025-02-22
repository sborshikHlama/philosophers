/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:26:18 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 16:12:34 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "stdio.h"

static void	assign_ordered_fork(t_philo *philo, t_fork *forks, int position)
{
	int		philo_num;
	t_fork	*right_fork;
	t_fork	*left_fork;

	philo_num = philo->sim->philo_num;
	right_fork = &forks[position];
	left_fork = &forks[(position + 1) % philo_num];
	philo->first_fork = right_fork;
	philo->second_fork = left_fork;
	if (philo->id % 2 == 0)
	{
		philo->first_fork = left_fork;
		philo->second_fork = right_fork;
	}
}

static int	init_philo(t_simulation *sim)
{
	long	i;
	t_philo	*philo;

	i = 0;
	while (i < sim->philo_num)
	{
		philo = sim->philos + i;
		philo->meals_counter = 0;
		philo->sim = sim;
		philo->full = 0;
		philo->id = i + 1;
		if (pthread_mutex_init(&philo->philo_mutex, NULL) != 0)
			return (MUTEX_INIT_ERROR);
		assign_ordered_fork(philo, sim->forks, i);
		i++;
	}
	return (SUCCESS);
}

static	int	init_forks(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->philo_num)
	{
		if (pthread_mutex_init(&sim->forks[i].fork, NULL) != 0)
			return (MUTEX_INIT_ERROR);
		sim->forks[i].id = i;
		i++;
	}
	return (SUCCESS);
}

int	init(t_simulation	*sim)
{
	sim->stop_flag = 0;
	sim->all_threads_ready = 0;
	sim->threads_running_num = 0;
	sim->error_flag = 0;
	sim->philos = malloc(sizeof(t_philo) * sim->philo_num);
	if (sim->philos == NULL)
		return (0);
	sim->forks = malloc(sizeof(t_fork) * sim->philo_num);
	if (sim->forks == NULL)
	{
		free(sim->philos);
		return (0);
	}
	if (pthread_mutex_init(&sim->sim_mutex, NULL) != 0)
		return (MUTEX_INIT_ERROR);
	if (pthread_mutex_init(&sim->write_mutex, NULL) != 0)
		return (MUTEX_INIT_ERROR);
	if (init_forks(sim) != SUCCESS)
		return (0);
	if (init_philo(sim) != SUCCESS)
		return (0);
	return (SUCCESS);
}
