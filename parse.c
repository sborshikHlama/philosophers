#include "philo.h"

static bool	inline ft_ispace(char c)
{
	return (c >= 9 && c <= 13 || c == 32);
}

char	*validate_str(char *str)
{
	unsigned	int	len;
	char	*num_str;

	while(ft_ispace(*str))
		str++;
	if (*str == '+')
		str++;
	else if(*str == '-')
		error_exit("Error: only positive numbers allowed");
	if (!ft_isdigit(*str))
		error_exit("Error: all inputs should be digits");
	num_str = str;
	while (ft_isdigit(*str++))
		len++;
	return (num_str);
}

long	ft_atol(char *str)
{
	long	num;

	str = validate_str(str);
	while (ft_isdigit(*str))
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	if (num > INT_MAX)
			error_exit("The value is too bigg");
	return (num);
}

void	parse_input(table_t	*table, char **argv)
{	
	table->philo_num = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]) * 1000;
	table->time_to_eat = ft_atol(argv[3]) * 1000;
	table->time_to_sleep = ft_atol(argv[4]) * 1000;
	if (table->time_to_die < 60000
		|| table->time_to_eat < 60000
		|| table->time_to_sleep < 60000)
		error_exit("Use timestamps more than 60ms");
	if (argv[5])
		table->limit_meals = ft_atol(argv[5]);
	else
		table->limit_meals = -1;
}
