#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <limits.h>
#include "libs/libft/libft.h"
#include <errno.h>

# define RED	"\033[1:31m"
# define GREEN	"\033[1:32m"

# define DEBUG_MODE 0

/*Fork*/
typedef struct s_fork
{
	int				id;
	pthread_mutex_t	fork;
}				t_fork;

/*Philosopher*/
typedef struct s_philo
{
	int				id;
	long			meals_counter;
	long			last_meal;
	t_fork			*first_fork;
	t_fork			*second_fork;
	bool			full;
	table_t			*table;
	pthread_t		thread_id;
	pthread_mutex_t	philo_mutex; //used for races with the monitor
}				t_philo;

/*Table*/
typedef struct s_table
{
	long	philo_num;
	long	time_to_die;
	long	time_to_sleep;
	long	time_to_eat;
	long	limit_meals;
	long	start_simulation;
	bool	all_threads_ready;
	bool	end_simulation;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	write_mutex;
	t_fork	*forks;
	t_philo	*philos;
}				table_t;


typedef enum e_status
{
	EATING,
	THINKING,
	SLEEPING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}			t_philo_status;

/*Codes for safe functions*/
typedef enum enum_operation
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	enum_opertaion_t;

/*Codes for gettime*/
typedef enum e_time_code
{
	SECOND,
	MILISECOND,
	MICROSECOND,
}		t_time_code;

void	parse_input(table_t	*table, char **argv);
void	error_exit(const char	*msg);
long	gettime(t_time_code time_code);
void 	precise_usleep(long usec, table_t *table);
/*functions from save_functions.c*/
void	*safe_malloc(size_t	bytes);
void	safe_thread_handle(pthread_t *thread, void *(*func)(void *), void * data,
			enum_opertaion_t operation);
void	safe_mutex_handle(pthread_mutex_t *mutex, enum_opertaion_t operation);
void	parse_input(table_t	*table, char **argv);

/*functions from init.c*/
void	data_init(table_t	*table);
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
bool	get_bool(pthread_mutex_t  *mutex, bool *value);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);
bool	simulation_finished(table_t *table);
/*synchro.c*/
void	wait_all_threads(table_t *table);

/*write.c*/
void	write_status(t_philo_status status, t_philo *philo, bool debug);
#endif