/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:03:58 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 16:26:58 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# define SYS_OK 0
# define LEFT 0
# define RIGHT 1
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_error
{
	OK = 0,
	E_NOMEM,
	E_INPUT_ARGC,
	E_INPUT_TYPE,
	E_THREAD_FAIL
}	t_error;

typedef enum e_action
{
	GRAB_FORK,
	IS_EATING,
	IS_SLEEPING,
	IS_THINKING,
	HAS_DIED
}	t_action;

typedef struct s_philo
{
	size_t			id;
	size_t			n_eaten;
	size_t			last_eaten;
	pthread_t		thread;
	struct s_data	*data;
	pthread_mutex_t	*pforks[2];
	pthread_mutex_t	last_eaten_lock;
}	t_philo;

typedef struct s_data
{
	bool			is_goal;
	bool			quit;
	size_t			n_phils;
	size_t			n_phils_to_eat;
	size_t			t_die;
	size_t			t_eat;
	size_t			t_sleep;
	size_t			n_to_eat;
	size_t			n_forks;
	t_philo			*philos;
	struct timeval	starttime;
	pthread_mutex_t	quit_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	n_phils_to_eat_lock;
	pthread_mutex_t	*forks;
}	t_data;

//
// destroy_mutexes.c

void	destroy_general_mutexes(t_data *data);
void	destroy_forks(t_data *data, size_t n_forks);
void	destroy_mutexes(t_data *data, size_t n_forks);

//
// error.c

int		error_and_exit(t_error error, int exit_status);
t_error	set_error(t_error error);
t_error	get_error(void);

//
// ft_atost_strict.c

bool	ft_atost_strict(char *str, size_t *num);

//
// init_mutexes.c

t_error	initialize_mutexes(t_data *data);

//
// init_philos

t_error	init_philos(t_data *data);

//
// monitor_philos.c

void	monitor_philos(t_philo *philos, t_data *data, int argc);

//
// philo_actions.c

void	philo_sleep(t_philo *philo);
void	philo_eat(t_philo *philo);
void	grab_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
void	cautious_sleep(size_t t_sleep, t_philo *philo);

//
// philo_utils.c

void	print_action(t_philo *philo, t_action action);
void	safe_print_action(t_philo *philo, t_action action);
void	set_quit(bool *quit, pthread_mutex_t *quit_lock, bool state);
size_t	convert_ms(struct timeval time);

//
// thread_func.c

void	*thread_func(void *philoptr);

#endif
