/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_func.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:33 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/27 14:19:09 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

static void	do_philo_loop(t_philo *philo)
{
	while (true)
	{
		grab_forks(philo);
		philo_eat(philo);
		drop_forks(philo);
		pthread_mutex_lock(&philo->data->quit_lock);
		if (philo->data->quit == true)
			break ;
		pthread_mutex_unlock(&philo->data->quit_lock);
		philo_sleep(philo);
		safe_print_action(philo, IS_THINKING);
		if (philo->data->n_phils % 2 == 1)
			cautious_sleep(200, philo);
		pthread_mutex_lock(&philo->data->quit_lock);
		if (philo->data->quit == true)
			break ;
		pthread_mutex_unlock(&philo->data->quit_lock);
	}
	pthread_mutex_unlock(&philo->data->quit_lock);
}

static void	single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->pforks[LEFT]);
	print_action(philo, GRAB_FORK);
	cautious_sleep(philo->data->t_die + 1, philo);
}

void	*thread_func(void *philoptr)
{
	t_philo	*philo;

	philo = philoptr;
	pthread_mutex_lock(&philo->data->start_lock);
	pthread_mutex_unlock(&philo->data->start_lock);
	if (philo->id % 2 == 0)
		cautious_sleep(philo->data->t_eat / 2, philo);
	if (philo->data->n_phils == 1)
	{
		single_philo(philo);
		return (NULL);
	}
	do_philo_loop(philo);
	return (NULL);
}
