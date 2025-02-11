/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:02:04 by aevstign          #+#    #+#             */
/*   Updated: 2025/02/11 15:23:17 by aevstign         ###   ########.fr       */
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

# define EPERM		1
# define ENOENT		2
# define ESRCH		3
# define EINTR		4
# define EIO		5
# define ENXIO		6
# define ENOMEM		12
# define EINVAL		22
# define EDEADLK	35
# define ECANCELED	125
# define EBUSY		16
# define EAGAIN		35

# define DEBUG_MODE	0

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
	long			last_meal_time;
	t_fork			*first_fork;
	t_fork			*second_fork;
	int				full;
	t_simulation	*sim;
	pthread_t		thread_id;
	t_mutex			philo_mutex;
}				t_philo;

typedef struct s_sim
{
	long			philo_num;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			meals_to_eat;
	long			start_simulation;
	int				all_threads_ready;
	int				stop_flag;
	int				threads_running_num;
	pthread_t		monitor;
	t_mutex			sim_mutex;
	t_mutex			write_mutex;
	t_fork			*forks;
	t_philo			*philos;
}				t_simulation;

typedef enum e_error_status
{
	MUTEX_INIT_ERROR,
	MUTEX_LOCK_ERROR,
	MUTEX_UNLOCK_ERROR,
	PTHREAD_CREATE_ERROR,
	SUCCESS,
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
long	gettime(t_time_code time_code);
void	precise_usleep(long usec);

void	clean(t_simulation *sim);

// simulation.c
void	simulate(t_simulation	*sim);
void	*philosopher(void *data);

// safe_functions.c
void	*safe_malloc(size_t	bytes);
void	safe_thread_op(pthread_t *thread, void *(*func)(void *), void *data,
			t_operation operation);
void	safe_mutex_op(t_mutex *mutex, t_operation operation);

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
int		all_threads_running(t_mutex *mutex,
			long *threads, long philo_num);
void	*monitor(void *data);
void	increase_long(t_mutex *mutex, long *value);

/*write.c*/
void	write_status(t_philo_status status, t_philo *philo, int debug);

int		inc_running_threads(t_mutex *mutex, int num);

#endif