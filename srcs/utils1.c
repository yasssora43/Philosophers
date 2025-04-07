/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 01:18:33 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 09:04:58 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	int		i;
	long	n;
	int		sign;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (n > (LONG_MAX - (str[i] - '0')) / 10)
			return (-sign * (sign == 1));
		n = n * 10;
		n += str[i] - '0';
		i++;
	}
	return ((int)(sign * n));
}

void	ft_putstr(const char *s, int fd)
{
	while (*s)
		write(fd, s++, 1);
}

int	is_valid_number(char *str)
{
	int		j;
	long	result;

	j = 0;
	result = 0;
	while (str[j] == ' ')
		j++;
	if (str[j] == '\0')
		return (-1);
	if (str[j] == '+' && str[j + 1] == '\0')
		return (-1);
	if (str[j] == '+')
		j++;
	while (str[j])
	{
		if (!ft_isdigit(str[j]))
			return (-1);
		if (result > (INT_MAX - (str[j] - '0')) / 10)
			return (-1);
		result = result * 10 + (str[j] - '0');
		j++;
	}
	return (0);
}

int	parse_args(int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		write(2, "Error: Incorrect number of arguments.\n", 37);
		return (1);
	}
	while (i < argc)
	{
		if (is_valid_number(argv[i]) != 0)
		{
			printf("Argument %d is not a valid number\n", i);
			return (1);
		}
		i++;
	}
	if (ft_atoi(argv[1]) < 1)
	{
		write(2, "Error: Number of philosophers must be at least 1.\n", 50);
		return (1);
	}
	return (0);
}
