/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:04 by aevstign          #+#    #+#             */
/*   Updated: 2024/12/08 21:56:58 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <errno.h>

# define RED       "\033[1;31m"
# define GREEN     "\033[1;32m"
# define YELLOW    "\033[1;33m"
# define BLUE      "\033[1;34m"
# define MAGENTA   "\033[1;35m"
# define CYAN      "\033[1;36m"
# define WHITE     "\033[1;37m"
# define BLACK     "\033[1;30m"
# define GRAY      "\033[0;37m"
# define RESET     "\033[0m"

# define DEBUG_MODE 1

typedef struct s_table	t_table;
typedef pthread_mutex_t	t_mutex;

typedef struct s_fork
{
	int				id;
	t_mutex			fork;
}				t_fork;

typedef struct s_philo
{
	int				id;
	long			meals_counter;
	long			last_meal_time;
	t_fork			*first_fork;
	t_fork			*second_fork;
	bool			full;
	t_table			*table;
	pthread_t		thread_id;
	t_mutex			philo_mutex; //used for races with the monitor
}				t_philo;

typedef struct s_table
{
	long			philo_num;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			limit_meals;
	long			start_simulation;
	bool			all_threads_ready;
	bool			end_simulation;
	long			threads_running_num;
	pthread_t		monitor;
	t_mutex			table_mutex;
	t_mutex			write_mutex;
	t_fork			*forks;
	t_philo			*philos;
}				t_table;

typedef enum e_status
{
	EATING,
	THINKING,
	SLEEPING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}			t_philo_status;

typedef enum enum_operation
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_operation;

typedef enum e_time_code
{
	SECOND,
	MILISECOND,
	MICROSECOND,
}		t_time_code;

// parse.c
void	parse_input(t_table	*table, char **argv);
void	error_exit(const char	*msg);
long	gettime(t_time_code time_code);
void	precise_usleep(long usec);

void	clean(t_table *table);

// safe_wrappers.cc
void	*safe_malloc(size_t	bytes);
void	safe_thread_op(pthread_t *thread, void *(*func)(void *), void *data,
			t_operation operation);
void	safe_mutex_op(t_mutex *mutex, t_operation operation);
void	parse_input(t_table	*table, char **argv);

/*functions from init.c*/
void	init(t_table	*table);

/*funtctions from getters_setters*/
void	set_bool(t_mutex *mutex, bool *dest, bool value);
bool	get_bool(t_mutex *mutex, bool *value);
void	set_long(t_mutex *mutex, long *dest, long value);
long	get_long(t_mutex *mutex, long *value);
bool	simulation_finished(t_table *table);

void	run_threads(t_table	*table);
void	think(t_philo *philo, bool pre_simulation);
void	de_sync(t_philo *philo);
/*synchro.c*/
void	wait_all_threads(t_table *table);
bool	all_threads_running(t_mutex *mutex,
			long *threads, long philo_num);

/*write.c*/
void	write_status(t_philo_status status, t_philo *philo, bool debug);
void	increase_long(t_mutex *mutex, long *value);
void	*monitor_dinner(void *data);
#endif