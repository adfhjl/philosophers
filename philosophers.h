/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 13:03:58 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/14 13:57:16 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h> // size_t
# include <stdbool.h> // bool

typedef enum e_error
{
	OK = 0,
	E_INPUT_ARGC,
	E_INPUT_TYPE
}	t_error;

typedef struct s_data
{
	size_t	n_phils;
	size_t	t_die;
	size_t	t_eat;
	size_t	t_sleep;
	size_t	n_to_eat;
	bool	is_goal;
}	t_data;

//
// error.c

int		error_and_exit(t_error error, int exit_status);
t_error	set_error(t_error error);
t_error	get_error(void);


#endif
