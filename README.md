## Process chart that shows deadlock from Operating Sytems by William Stallings

![Screenshot 2025-02-12 at 17 16 00](https://github.com/user-attachments/assets/ba7b18c1-42c8-4d70-87a8-c354855a8dce)

## Problem of asymmetry:

Asymmetry creates contention between philosophers and this contention is not fair, because one philosopher can eat more often then necessary and other can starve.
How to make dinner more fair: add thinking time so after philosopher ate, it should think a bit, so the one who was waiting for fork could eat.

## My philosophers problem solution:

Deadlock prevention using direct prevention method, also knows as asymmetry. It breaks circular wait condition. I make even philosophers take right fork and odd philosophers to take left fork.

| Problem             | Basic Dining Philosophers                                 | My Implementation                                                     |
| ------------------- | --------------------------------------------------------- | --------------------------------------------------------------------- |
| **Starvation**      | Some philosophers can eat repeatedly while others starve. | Thinking time adapts based on starvation risk, preventing starvation. |
| **Fork Hogging**    | Philosophers who eat fast can quickly try again.          | Philosophers who just ate are forced to wait longer.                  |
| **Race Conditions** | All philosophers attempt to grab forks at the same time.  | Thinking time staggers philosophers, reducing contention.             |
| **Fairness**        | No built-in priority for starving philosophers.           | Philosophers with less survival time get priority.                    |

The code that I used to make system less random and more fair.
int silent is used as 1 only when system starts and odd numbered philosophers has imitation of think time, so even philosophers would be always first
and system would be more fair.

```
static int	think(t_philo *philo, int silent)
{
	long	time_to_think;

	if (pthread_mutex_lock(&philo->philo_mutex) != 0)
		return (MUTEX_LOCK_ERROR);
	time_to_think = (philo->sim->time_to_die
			- (gettime(MILISECOND) - philo->last_meal_time)
			- philo->sim->time_to_eat) / 2;
	if (pthread_mutex_unlock(&philo->philo_mutex) != 0)
		return (MUTEX_UNLOCK_ERROR);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent > 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == 0)
		write_status(THINKING, philo, DEBUG_MODE);
	precise_usleep(time_to_think);
	return (SUCCESS);
}
```
