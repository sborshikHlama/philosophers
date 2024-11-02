#include "philo.h"

//TODO: rename file to safe_functions
// Memory functions with wrapper for  preventing memory leaks

void	*safe_malloc(size_t	bytes)
{
	void	*return_value;

	return_value = malloc(bytes);
	if (return_value == NULL)
		error_exit("Couldn't allocate memory");
	return (return_value);
}

static	void	handle_threads_error(int status, enum_opertaion_t opeation)
{
	if (status == 0)
		return ;
	else if (status == EAGAIN)
		error_exit("The system lacked the necessary resources to create another thread,\
                        or the system-imposed limit on the total number of threads.");
	else if (status == EINVAL && opeation == CREATE)
		error_exit("The value specified by attr is invalid.");
	else if (status == EPERM)
		error_exit("The caller does not have appropriate permission to set the required\
                        scheduling parameters or scheduling policy.");
	else if (status == EINVAL && (opeation == JOIN || opeation == DETACH))
		error_exit("The implementation has detected that the value specified by thread\
                        does not refer to a joinable thread.");
	else if (status == ESRCH)
		error_exit("No thread could be found corresponding to that specified by the\
                        given thread ID, thread.");
	else if (status == EDEADLK)
		error_exit("A deadlock was detected or the value of thread specifies the\
                        calling thread.");
}

/*Tracking and displaying internal mutex errors*/
static	void	handle_mutex_error(int status, enum_opertaion_t operation)
{
	if (status == 0)
		return ;
	if (status == 	EINVAL && (operation == LOCK
		|| operation == UNLOCK || operation == DESTROY))
		error_exit("The value specified by mutex is invalid.");
	else if (status == EDEADLK)
		error_exit("A deadlock would occur if the thread blocked waiting for mutex.");
	else if (status == EINVAL && operation == INIT)
		error_exit("The value specified by attr is invalid.");
	else if (status == ENOMEM)
		error_exit("The process cannot allocate enough memory to create another mutex.");
	else if (status == EPERM)
		error_exit("The current thread does not hold a lock on mutex.");
	else if (status == EBUSY)
		error_exit("Mutex is locked.");
}

void	safe_thread_handle(pthread_t *thread, void *(*func)(void *), void * data,
	enum_opertaion_t operation)
{
	if (operation == CREATE)
		handle_threads_error(pthread_create(thread, NULL, func, data), operation);
	else if (operation == JOIN)
		handle_threads_error(pthread_join(*thread, NULL), operation);
	else if (operation == DETACH)
		handle_threads_error(pthread_detach(*thread), operation);
	else
		error_exit("Error: wrong thread operation");
}

/*Using enums to handle mutex operations and possible errors at one place in programm*/
void	safe_mutex_handle(pthread_mutex_t *mutex, enum_opertaion_t operation)
{
	if (operation == INIT)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), operation);
	else if (operation == LOCK)
		handle_mutex_error(pthread_mutex_lock(mutex), operation);
	else if (operation == UNLOCK)
		handle_mutex_error(pthread_mutex_unlock(mutex), operation);
	else if (operation == DESTROY)
		handle_mutex_error(pthread_mutex_destroy(mutex), operation);
	else
		error_exit("Error: wrong mutex operation");
}
