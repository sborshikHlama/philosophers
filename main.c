#include "philo.h"

void	clean(table_t *table)
{
	int i;
	t_philo	*philo;

	i = 0;
	while(i < table->philo_num)
	{
		philo = table->philos + i;
		safe_mutex_handle(&philo->philo_mutex, DESTROY);
		i++;
	}
	safe_mutex_handle(&table->write_mutex, DESTROY);
	safe_mutex_handle(&table->table_mutex, DESTROY);
}

int	main(int argc, char **argv)
{
	table_t		table;

	if (argc == 5 || argc == 6)
	{
		parse_input(&table, argv);
		data_init(&table);
		dinner_start(&table);
		clean(&table);
	}
	else
		error_exit("Error: wrong number of args");
}
