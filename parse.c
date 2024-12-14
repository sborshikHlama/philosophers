/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:04:17 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/14 17:23:46 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

char	*validate_str(char *str)
{
	char	*num_str;

	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_exit("Error: only positive numbers allowed");
	if (!is_digit(*str))
		error_exit("Error: all inputs should be digits");
	num_str = str;
	return (num_str);
}

long	ft_atol(char *str)
{
	long	num;

	num = 0;
	str = validate_str(str);
	while (is_digit(*str))
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	if (num > 2147483647)
		error_exit("The value is too bigg");
	return (num);
}

void	parse_input(t_simulation	*sim, char **argv)
{
	sim->philo_num = ft_atol(argv[1]);
	sim->time_to_die = ft_atol(argv[2]) * 1e3;
	sim->time_to_eat = ft_atol(argv[3]) * 1e3;
	sim->time_to_sleep = ft_atol(argv[4]) * 1e3;
	if (sim->time_to_die < 6e4
		|| sim->time_to_eat < 6e4
		|| sim->time_to_sleep < 6e4)
		error_exit("Use timestamps more than 60ms");
	if (argv[5])
		sim->meals_to_eat = ft_atol(argv[5]);
	else
		sim->meals_to_eat = -1;
}
