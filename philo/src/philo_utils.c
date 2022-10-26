/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:31 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 16:21:05 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>

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
	printf("[%zu] %zu %s\n", convert_ms(time)
		- convert_ms(philo->data->starttime), philo->id, arr[action]);
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

size_t	convert_ms(struct timeval time)
{
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
