/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_actions.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/26 15:07:27 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 15:19:36 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

void	philo_sleep(t_philo *philo)
{
	safe_print_action(philo, IS_SLEEPING);
	cautious_sleep(philo->data->t_eat, philo);
}

void	philo_eat(t_philo *philo)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	pthread_mutex_lock(&philo->last_eaten_lock);
	philo->last_eaten = convert_ms(time);
	pthread_mutex_unlock(&philo->last_eaten_lock);
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

void	cautious_sleep(size_t t_sleep, t_philo *philo)
{
	struct timeval	begin;
	struct timeval	current;

	gettimeofday(&begin, NULL);
	gettimeofday(&current, NULL);
	while (convert_ms(current) - convert_ms(begin) < t_sleep)
	{
		pthread_mutex_lock(&philo->data->quit_lock);
		if (philo->data->quit == true)
		{
			pthread_mutex_unlock(&philo->data->quit_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->data->quit_lock);
		usleep(500);
		gettimeofday(&current, NULL);
	}
}
