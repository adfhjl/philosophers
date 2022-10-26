/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor_philos.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:25 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 15:48:59 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

static void	check_well_fed(t_data *data)
{
	pthread_mutex_lock(&data->n_phils_to_eat_lock);
	if (data->n_phils_to_eat == 0)
		set_quit(&data->quit, &data->quit_lock, true);
	pthread_mutex_unlock(&data->n_phils_to_eat_lock);
}

static void	check_philos_death(t_philo *philos, t_data *data)
{
	size_t			i;
	struct timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	while (i < data->n_phils)
	{
		pthread_mutex_lock(&philos[i].last_eaten_lock);
		if (convert_ms(time) >= data->t_die + philos[i].last_eaten)
		{
			pthread_mutex_unlock(&philos[i].last_eaten_lock);
			set_quit(&data->quit, &data->quit_lock, true);
			print_action(&philos[i], HAS_DIED);
			return ;
		}
		pthread_mutex_unlock(&philos[i].last_eaten_lock);
		i++;
	}
}

void	monitor_philos(t_philo *philos, t_data *data, int argc)
{
	pthread_mutex_lock(&data->quit_lock);
	while (data->quit != true)
	{
		pthread_mutex_unlock(&data->quit_lock);
		check_philos_death(philos, data);
		if (argc == 6)
			check_well_fed(data);
		pthread_mutex_lock(&data->quit_lock);
	}
	pthread_mutex_unlock(&data->quit_lock);
}
