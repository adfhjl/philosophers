/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 14:19:30 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/18 14:26:02 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

void	*test(void *ptr)
{
	static int		i = 0;
	pthread_mutex_t	*start;

	start = ptr;
	pthread_mutex_lock(start);
	ft_printf("%d\n", i);
	i++;
	pthread_mutex_unlock(start);
	return (NULL);
}

int	main(void)
{
	pthread_mutex_t	start;
	pthread_t		thread;

	pthread_mutex_init(&start, NULL);
	pthread_mutex_lock(&start);
	for (int i = 0; i < 3; i++)
	{
		pthread_create(&thread, NULL, &test, &start);
	}
	pthread_mutex_unlock(&start);
	return (0);
}
