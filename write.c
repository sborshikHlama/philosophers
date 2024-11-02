#include "philo.h"

/*Debuggin 🐞 function that adds more info to the output*/
static	void	write_status_debug(t_philo_status status, t_philo *philo, long elapsed)
{		
		if ((status == TAKE_FIRST_FORK)
			&& !simulation_finished(philo->table))
			printf(WHITE"⏱️ %-6ld %d has taken the first fork 🍴[%d]🍴\n"RESET,
				elapsed, philo->id, philo->first_fork->id);
		else if (status == TAKE_SECOND_FORK && !simulation_finished(philo->table))
			printf(WHITE"⏱️ %-6ld %d has taken the second fork 🍴[%d]🍴\n"RESET,
				elapsed, philo->id, philo->second_fork->id);
		else if (status == EATING && !simulation_finished(philo->table))
			printf(GREEN"⏱️ %-6ld %d is eating 🍜[%ld]🍜\n"RESET, elapsed,
				philo->id, philo->meals_counter);
		else if (status == SLEEPING && !simulation_finished(philo->table))
			printf(BLUE"⏱️ %-6ld %d is sleeping 😴😴😴\n"RESET, elapsed, philo->id);
		else if (status == THINKING && !simulation_finished(philo->table))
			printf(YELLOW"⏱️ %-6ld %d is thinking\n"RESET, elapsed, philo->id);
		else if (status == DIED && !simulation_finished(philo->table))
			printf(RED"⏱️ %-6ld %d  💀💀💀died💀💀💀\n"RESET, elapsed, philo->id);
}

/* write [time_ms] [philo_id] [action]*/
void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	elapsed = gettime(MILISECOND) - philo->table->start_simulation;
	if (philo->full)
		return ;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !simulation_finished(philo->table))
			printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
		else if (status == EATING && !simulation_finished(philo->table))
			printf(GREEN"%-6ld %d is eating\n"RESET, elapsed, philo->id);
		else if (status == SLEEPING && !simulation_finished(philo->table))
			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
		else if (status == THINKING && !simulation_finished(philo->table))
			printf("%-6ld %d is thinking\n", elapsed, philo->id);
		else if (status == DIED && !simulation_finished(philo->table))
			printf(RED"%-6ld %d  died\n"RESET, elapsed, philo->id);
	}
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}
