/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:04 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/22 11:34:11 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

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

typedef struct s_sim	t_simulation;
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
	time_t			last_meal_time;
	t_fork			*first_fork;
	t_fork			*second_fork;
	int				full;
	t_simulation	*sim;
	pthread_t		thread_id;
	t_mutex			philo_mutex;
}				t_philo;

typedef struct s_sim
{
	int				philo_num;
	time_t			time_to_die;
	time_t			time_to_sleep;
	time_t			time_to_eat;
	time_t			start_simulation;
	int				meals_to_eat;
	int				all_threads_ready;
	int				stop_flag;
	int				threads_running_num;
	int				error_flag;
	pthread_t		monitor;
	t_mutex			sim_mutex;
	t_mutex			write_mutex;
	t_fork			*forks;
	t_philo			*philos;
}				t_simulation;

typedef enum e_error_status
{
	MUTEX_INIT_ERROR = 2,
	MUTEX_LOCK_ERROR = 3,
	MUTEX_UNLOCK_ERROR = 4,
	MUTEX_WRITE_ERROR = 5,
	PTHREAD_CREATE_ERROR = 6,
	PTHREAD_JOIN_ERROR = 7,
	SUCCESS = 8,
}			t_error_status;

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
int		parse_input(t_simulation	*sim, char **argv);
int		ft_error(const char	*msg);

// utils.c
time_t	gettime_ms(void);
void	precise_usleep(t_simulation *sim, time_t sleep_time);

void	clean(t_simulation *sim);

// simulation.c
void	*philosopher(void *data);

/*functions from init.c*/
int		init(t_simulation	*sim);

/*funtctions from getters_setters*/
void	set_int(t_mutex *mutex, int *dest, int value);
int		get_int(t_mutex *mutex, int *value);
void	set_long(t_mutex *mutex, long *dest, long value);
long	get_long(t_mutex *mutex, long *value);
int		simulation_finished(t_simulation *sim);

/*synchro.c*/
void	wait_all_threads(t_simulation *sim);
void	*monitor(void *data);
void	increase_long(t_mutex *mutex, long *value);

/*write.c*/
int		write_status(t_philo_status status, t_philo *philo);
int		increase_int(t_mutex *mutex, int *num);

// main.c
int		handle_error(t_error_status status);

#endif