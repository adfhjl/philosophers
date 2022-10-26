/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:05:33 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 15:47:45 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

static t_error	initialize_data(t_data *data, int argc, char *argv[])
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

static void	wait_for_philos(t_philo *philos, size_t n_phils)
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

static void	run_philos(t_data *data)
{
	size_t	i;
	size_t	t;

	i = 0;
	pthread_mutex_lock(&data->start_lock);
	while (i < data->n_phils)
	{
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
	gettimeofday(&data->starttime, NULL);
	t = convert_ms(data->starttime);
	while (i > 0)
	{
		i--;
		data->philos[i].last_eaten = t;
	}
	pthread_mutex_unlock(&data->start_lock);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (error_and_exit(E_INPUT_ARGC, EXIT_FAILURE));
	if (initialize_data(&data, argc, argv) != OK)
		return (error_and_exit(get_error(), EXIT_FAILURE));
	if (init_philos(&data) != OK)
	{
		destroy_mutexes(&data, data.n_forks);
		return (error_and_exit(get_error(), EXIT_FAILURE));
	}
	run_philos(&data);
	monitor_philos(data.philos, &data, argc);
	wait_for_philos(data.philos, data.n_phils);
	destroy_mutexes(&data, data.n_forks);
	return (EXIT_SUCCESS);
}
