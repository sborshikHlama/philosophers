/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:26:18 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 01:02:50 by aevstign         ###   ########.fr       */
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
	if (philo->id % 2 == 0)
	{
		philo->first_fork = right_fork;
		philo->second_fork = left_fork;
	}
	else
	{
		philo->first_fork = left_fork;
		philo->second_fork = right_fork;
	}
}

static void	init_philo(t_simulation *sim)
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
		safe_mutex_op(&philo->philo_mutex, INIT);
		assign_ordered_fork(philo, sim->forks, i);
		i++;
	}
}

void	init(t_simulation	*sim)
{
	int	i;

	i = 0;
	sim->end_simulation = 0;
	sim->all_threads_ready = 0;
	sim->threads_running_num = 0;
	sim->philos = safe_malloc(sizeof(t_philo) * sim->philo_num);
	sim->forks = safe_malloc(sizeof(t_fork) * sim->philo_num);
	safe_mutex_op(&sim->sim_mutex, INIT);
	safe_mutex_op(&sim->write_mutex, INIT);
	while (i < sim->philo_num)
	{
		safe_mutex_op(&sim->forks[i].fork, INIT);
		sim->forks[i].id = i;
		i++;
	}
	init_philo(sim);
}
