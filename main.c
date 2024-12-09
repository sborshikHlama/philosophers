/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:51 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/08 21:56:58 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philo_num)
	{
		philo = table->philos + i;
		safe_mutex_op(&philo->philo_mutex, DESTROY);
		i++;
	}
	safe_mutex_op(&table->write_mutex, DESTROY);
	safe_mutex_op(&table->table_mutex, DESTROY);
}

int	main(int argc, char **argv)
{
	t_table		table;

	if (argc == 5 || argc == 6)
	{
		parse_input(&table, argv);
		init(&table);
		run_threads(&table);
		clean(&table);
	}
	else
		error_exit("Error: wrong number of args");
}
