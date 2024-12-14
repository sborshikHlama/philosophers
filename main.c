/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:51 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 03:14:31 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_simulation *sim)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < sim->philo_num)
	{
		philo = sim->philos + i;
		safe_mutex_op(&philo->philo_mutex, DESTROY);
		i++;
	}
	safe_mutex_op(&sim->write_mutex, DESTROY);
	safe_mutex_op(&sim->sim_mutex, DESTROY);
	free(sim->philos);
	free(sim->forks);
	sim->philos = NULL;
	sim->forks = NULL;
}
//TODO: Makefiel, philo.h

int	main(int argc, char **argv)
{
	t_simulation		sim;

	if (argc == 5 || argc == 6)
	{
		parse_input(&sim, argv);
		init(&sim);
		simulate(&sim);
		clean(&sim);
	}
	else
		error_exit("Error: wrong number of args");
}
