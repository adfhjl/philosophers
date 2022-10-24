/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:05:33 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/20 15:42:09 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "libft.h"

#include <pthread.h> // pthread_mutex_init(), pthread_mutex_t

t_error	initialize_fork_mutexes(t_data *data)
{
	size_t	i;

	data->forks = malloc(data->n_forks * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
		return (set_error(E_NOMEM));
	i = 0;
	while (i < data->n_forks)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL) != SYS_OK)
		{
			destroy_forks(data, i - 1);
			return (set_error(E_NOMEM));
		}
		i++;
	}
	return (OK);
}

t_error	initialize_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->quit_lock, NULL) != SYS_OK)
		return (set_error(E_NOMEM));
	if (pthread_mutex_init(&data->print_lock, NULL) != SYS_OK)
	{
		pthread_mutex_destroy(&data->quit_lock);
		return (set_error(E_NOMEM));
	}
	if (pthread_mutex_init(&data->n_phils_to_eat_lock, NULL) != SYS_OK)
	{
		pthread_mutex_destroy(&data->quit_lock);
		pthread_mutex_destroy(&data->print_lock);
	}
	if (initialize_fork_mutexes(data) != OK)
	{
		pthread_mutex_destroy(&data->quit_lock);
		pthread_mutex_destroy(&data->print_lock);
		pthread_mutex_destroy(&data->n_phils_to_eat_lock);
		return (get_error());
	}
	return (OK);
}

t_error	initialize_data(t_data *data, int argc, char *argv[])
{
	bool	status;

	status = true;
	status = (status && ft_atost_strict(argv[1], &data->n_phils));
	status = (status && ft_atost_strict(argv[2], &data->t_die));
	status = (status && ft_atost_strict(argv[3], &data->t_eat));
	status = (status && ft_atost_strict(argv[4], &data->t_sleep));
	data->is_goal = false;
	if (argc == 6)
	{
		status = (status && ft_atost_strict(argv[5], &data->n_to_eat));
		data->is_goal = true;
	}
	if (status == false || data->n_phils == 0 || data->t_die == 0 || \
	data->t_eat == 0 || data->t_sleep == 0 || \
	(data->is_goal && data->n_to_eat == 0))
		return (set_error(E_INPUT_TYPE));
	data->n_forks = data->n_phils;
	if (initialize_mutexes(data) != OK)
		return (get_error());
	data->quit = false;
	data->n_phils_to_eat = data->n_phils;
	return (OK);
}

void	wait_for_philos(t_philo *philos, size_t n_phils)
{
	size_t	i;

	i = 0;
	while (i < n_phils)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free(philos);
}

void	destroy_forks(t_data *data, size_t n_forks)
{
	size_t	i;

	i = 0;
	while (i < n_forks)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

void	destroy_mutexes(t_data *data, size_t n_forks)
{
	destroy_forks(data, n_forks);
	pthread_mutex_destroy(&data->quit_lock);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->n_phils_to_eat_lock);
}

void	check_well_fed(t_data *data)
{
	pthread_mutex_lock(&data->quit_lock);
	while (data->quit != true)
	{
		pthread_mutex_unlock(&data->quit_lock);
		pthread_mutex_lock(&data->n_phils_to_eat_lock);
		if (data->n_phils_to_eat == 0)
			set_quit(&data->quit, &data->quit_lock, true);
		pthread_mutex_unlock(&data->n_phils_to_eat_lock);
		pthread_mutex_lock(&data->quit_lock);
	}
	pthread_mutex_unlock(&data->quit_lock);
}

#include <stdlib.h> // EXIT_STATUS

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (error_and_exit(E_INPUT_ARGC, EXIT_FAILURE));
	if (initialize_data(&data, argc, argv) != OK)
		return (error_and_exit(get_error(), EXIT_FAILURE));
	run_philos(&data);
	if (argc == 6)
		check_well_fed(&data);
	wait_for_philos(data.philos, data.n_phils);
	destroy_forks(&data, data.n_forks);
	return (EXIT_SUCCESS);
}

void	*thread_func(void *philoptr)
{
	t_philo	*philo;

	philo = philoptr;
	pthread_mutex_lock(&philo->data->quit_lock);
	while (philo->data->quit != true)
	{
		pthread_mutex_unlock(&philo->data->quit_lock);
		if (philo->data->n_phils == 1)
		{
			single_philo(philo);
			return (NULL);
		}
		grab_forks(philo);
		philo_eat(philo);
		drop_forks(philo);
		pthread_mutex_lock(&philo->data->quit_lock);
		if (philo->data->quit == true)
			break ;
		pthread_mutex_unlock(&philo->data->quit_lock);
		philo_sleep(philo);
		safe_print_action(philo, IS_THINKING);
		pthread_mutex_lock(&philo->data->quit_lock);
	}
	pthread_mutex_unlock(&philo->data->quit_lock);
	return (NULL);
}

void	single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->pforks[LEFT]);
	print_action(philo, GRAB_FORK);
	cautious_sleep(philo->data->t_die + 1, philo);
}

void	philo_sleep(t_philo *philo)
{
	safe_print_action(philo, IS_SLEEPING);
	cautious_sleep(philo->data->t_eat, philo);
}

void	philo_eat(t_philo *philo)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	philo->last_eaten = time.tv_sec * 1000 + time.tv_usec / 1000;
	safe_print_action(philo, IS_EATING);
	philo->n_eaten++;
	if (philo->n_eaten == philo->data->n_to_eat)
	{
		pthread_mutex_lock(&philo->data->n_phils_to_eat_lock);
		philo->data->n_phils_to_eat--;
		pthread_mutex_unlock(&philo->data->n_phils_to_eat_lock);
	}
	cautious_sleep(philo->data->t_eat, philo);
}

void	grab_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->pforks[RIGHT]);
		safe_print_action(philo, GRAB_FORK);
		pthread_mutex_lock(philo->pforks[LEFT]);
		safe_print_action(philo, GRAB_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->pforks[LEFT]);
		safe_print_action(philo, GRAB_FORK);
		pthread_mutex_lock(philo->pforks[RIGHT]);
		safe_print_action(philo, GRAB_FORK);
	}
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->pforks[RIGHT]);
		pthread_mutex_unlock(philo->pforks[LEFT]);
	}
	else
	{
		pthread_mutex_unlock(philo->pforks[LEFT]);
		pthread_mutex_unlock(philo->pforks[RIGHT]);
	}
}

#include <sys/time.h>

void	cautious_sleep(size_t t_sleep, t_philo *philo)
{
	struct timeval	begin;
	struct timeval	current;

	gettimeofday(&begin, NULL);
	gettimeofday(&current, NULL);
	while ((size_t)((current.tv_sec * 1000 + current.tv_usec / 1000) - \
	(begin.tv_sec * 1000 + begin.tv_usec / 1000)) < t_sleep)
	{
		pthread_mutex_lock(&philo->data->quit_lock);
		if (philo->data->quit == true)
		{
			pthread_mutex_unlock(&philo->data->quit_lock);
			return ;
		}
		if ((current.tv_sec * 1000 + current.tv_usec / 1000) \
		- philo->last_eaten >= philo->data->t_die)
		{
			philo->data->quit = true;
			print_action(philo, HAS_DIED);
			pthread_mutex_unlock(&philo->data->quit_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->data->quit_lock);
		usleep(500);
		gettimeofday(&current, NULL);
	}
}

void	init_philo(t_philo *philo, size_t i, t_data *data)
{
	struct timeval	time;

	philo->pforks[LEFT] = &data->forks[i];
	philo->pforks[RIGHT] = &data->forks[(i + 1) % data->n_phils];
	philo->data = data;
	philo->n_eaten = 0;
	philo->id = i + 1;
	gettimeofday(&time, NULL);
	philo->last_eaten = time.tv_sec * 1000 + time.tv_usec / 1000;
}

void	run_philos(t_data *data)
{
	size_t	i;

	i = 0;
	data->philos = malloc(data->n_phils * sizeof(t_philo));
	if (data->philos == NULL)
	{
		set_error(E_NOMEM);
		data->n_phils = 0;
		return ;
	}
	gettimeofday(&data->starttime, NULL);
	while (i < data->n_phils)
	{
		init_philo(&data->philos[i], i, data);
		if (pthread_create(&(data->philos[i].thread), NULL, &thread_func, \
		&data->philos[i]) != SYS_OK)
		{
			set_quit(&data->quit, &data->quit_lock, true);
			data->n_phils = i;
			set_error(E_THREAD_FAIL);
			break ;
		}
		i++;
	}
}

#include <stdio.h> // printf()

void	print_action(t_philo *philo, t_action action)
{
	struct timeval	time;
	const char		*arr[] = {
	[GRAB_FORK] = "has taken a fork",
	[IS_EATING] = "is eating",
	[IS_SLEEPING] = "is sleeping",
	[IS_THINKING] = "is thinking",
	[HAS_DIED] = "died"
	};

	pthread_mutex_lock(&philo->data->print_lock);
	gettimeofday(&time, NULL);
	printf("[%zu] %zu %s\n", (time.tv_sec * 1000 + time.tv_usec / 1000) - (philo->data->starttime.tv_sec * 1000 + philo->data->starttime.tv_usec / 1000), \
	philo->id, arr[action]);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	safe_print_action(t_philo *philo, t_action action)
{
	pthread_mutex_lock(&philo->data->quit_lock);
	if (philo->data->quit != true)
		print_action(philo, action);
	pthread_mutex_unlock(&philo->data->quit_lock);
}

void	set_quit(bool *quit, pthread_mutex_t *quit_lock, bool state)
{
	pthread_mutex_lock(quit_lock);
	*quit = state;
	pthread_mutex_unlock(quit_lock);
}
