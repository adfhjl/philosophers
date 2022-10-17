/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:10:56 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/14 17:44:39 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "libft.h"

static t_error	*get_error_ptr(void)
{
	static t_error	err = OK;

	return (&err);
}

t_error	get_error(void)
{
	return (*get_error_ptr());
}

t_error	set_error(t_error error)
{
	*get_error_ptr() = error;
	return (error);
}

int	error_and_exit(t_error error, int exit_status)
{
	const char	*arr[] = {
	[OK] = "Exiting with t_error set to OK\n",
	[E_INPUT_ARGC] = "philosophers: Expected 4 or 5 arguments\n",
	[E_INPUT_TYPE] = "philosophers: Expected a whole number\n"
	};

	write(STDERR_FILENO, arr[error], ft_strlen(arr[error]));
	return (exit_status);
}
