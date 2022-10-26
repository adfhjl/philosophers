/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atost_strict.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: vbenneko <vbenneko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 15:51:51 by vbenneko      #+#    #+#                 */
/*   Updated: 2022/10/26 15:39:01 by vbenneko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static char	skip_space(char *str, size_t *i)
{
	while (str[*i] == ' ')
		(*i)++;
	return (str[*i]);
}

bool	ft_atost_strict(char *str, size_t *num)
{
	const size_t	overflow_limit = INT64_MAX / 10;
	const int		overflow_digit = INT64_MAX % 10;
	size_t			i;

	i = 0;
	*num = 0;
	if (skip_space(str, &i) == '\0')
		return (false);
	if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		if (*num > overflow_limit || \
		(*num == overflow_limit && str[i] - '0' > overflow_digit))
		{
			*num = 0;
			return (false);
		}
		*num = (*num * 10) + (str[i] - '0');
		i++;
	}
	if (skip_space(str, &i) == '\0')
		return (true);
	*num = 0;
	return (false);
}
