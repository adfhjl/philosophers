/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:05:33 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/17 14:34:14 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "libft.h"
#include <stdlib.h> // EXIT_STATUS

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
	if (status == false)
		return (set_error(E_INPUT_TYPE));
	return (OK);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (error_and_exit(E_INPUT_ARGC, EXIT_FAILURE));
	if (initialize_data(&data, argc, argv) != OK)
		return (error_and_exit(get_error(), EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
