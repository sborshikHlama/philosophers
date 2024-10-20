#include "philo.h"

int	main(int argc, char **argv)
{
	table_t		table;
	if (argc == 5 || argc == 6)
	{
		parse_input(&table, argv);
		//TODO
		data_init(&table);
		//TODO
		// start_dinner(&table);
		// //TODO
		// clean(&table);
	}
	else
		error_exit("Error: wrong number of args");
}
