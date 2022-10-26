/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_philos.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:22 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 15:46:53 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

static t_error	init_philo(t_philo *philo, size_t i, t_data *data)
{
	struct timeval	time;

	philo->pforks[LEFT] = &data->forks[i];
	philo->pforks[RIGHT] = &data->forks[(i + 1) % data->n_phils];
	philo->data = data;
	philo->n_eaten = 0;
	philo->id = i + 1;
	gettimeofday(&time, NULL);
	philo->last_eaten = convert_ms(time);
	if (pthread_mutex_init(&philo->last_eaten_lock, NULL) != SYS_OK)
		return (set_error(E_NOMEM));
	return (OK);
}

t_error	init_philos(t_data *data)
{
	size_t	i;

	data->philos = malloc(data->n_phils * sizeof(t_philo));
	if (data->philos == NULL)
	{
		data->n_phils = 0;
		return (set_error(E_NOMEM));
	}
	i = 0;
	while (i < data->n_phils)
	{
		if (init_philo(&data->philos[i], i, data) != OK)
		{
			while (i != 0)
			{
				i--;
				pthread_mutex_destroy(&data->philos[i].last_eaten_lock);
			}
			return (get_error());
		}
		i++;
	}
	return (OK);
}
