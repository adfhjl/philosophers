/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   destroy_mutexes.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:16 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 16:28:41 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdlib.h>

void	destroy_general_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->quit_lock);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->n_phils_to_eat_lock);
	pthread_mutex_destroy(&data->start_lock);
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
	destroy_general_mutexes(data);
}
