/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:04:17 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/11 14:52:33 by aevstign         ###   ########.fr       */
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
	{
		ft_error("Error: only positive numbers allowed");
		return (NULL);
	}
	if (!is_digit(*str))
	{
		ft_error("Error: all inputs should be digits");
		return (NULL);
	}
	num_str = str;
	return (num_str);
}

long	ft_atol(char *str)
{
	long	num;

	num = 0;
	str = validate_str(str);
	if (str == NULL)
		return (-1);
	while (is_digit(*str))
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	if (num > 2147483647)
	{
		ft_error("The value is too big");
		return (-1);
	}
	return (num);
}

int	parse_input(t_simulation	*sim, char **argv)
{
	sim->philo_num = ft_atol(argv[1]);
	if (sim->philo_num < 0)
		return (0);
	sim->time_to_die = ft_atol(argv[2]) * 1e3;
	sim->time_to_eat = ft_atol(argv[3]) * 1e3;
	sim->time_to_sleep = ft_atol(argv[4]) * 1e3;
	if (sim->time_to_die < 6e4
		|| sim->time_to_eat < 6e4
		|| sim->time_to_sleep < 6e4)
	{
		ft_error("Use timestamps more than 60ms");
		return (0);
	}
	if (argv[5])
	{
		sim->meals_to_eat = ft_atol(argv[5]);
		if (sim->meals_to_eat < 0)
			return (0);
	}
	else
		sim->meals_to_eat = -1;
	return (SUCCESS);
}
