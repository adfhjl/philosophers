/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_mutexes.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:20 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 16:28:21 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static t_error	initialize_fork_mutexes(t_data *data)
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

static t_error	initialize_general_mutexes(t_data *data)
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
		return (set_error(E_NOMEM));
	}
	if (pthread_mutex_init(&data->start_lock, NULL) != SYS_OK)
	{
		pthread_mutex_destroy(&data->quit_lock);
		pthread_mutex_destroy(&data->print_lock);
		pthread_mutex_destroy(&data->n_phils_to_eat_lock);
		return (set_error(E_NOMEM));
	}
	return (OK);
}

t_error	initialize_mutexes(t_data *data)
{
	if (initialize_general_mutexes(data) != OK)
		return (get_error());
	if (initialize_fork_mutexes(data) != OK)
	{
		destroy_general_mutexes(data);
		return (get_error());
	}
	return (OK);
}
