/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:04:33 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:52:46 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_putnbr(long long n, int fd)
{
	char	c;

	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

void	print_status(t_philo *philos, const char *status)
{
	long long	time;

	pthread_mutex_lock(&philos->data->print_lock);
	time = get_time_ms() - philos->data->start_time;
	if (!check_simulation_over(philos))
	{
		printf("%lld %d %s \n", time, philos->id, status);
	}
	pthread_mutex_unlock(&philos->data->print_lock);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	handle_single_philo(t_philo *philos)
{
	print_status(philos, "has taken a fork");
	precise_sleep(philos->data->time_to_die);
	return ;
}

void	free_mutexes_and_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
		pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&data->start_time_lock);
	pthread_mutex_destroy(&data->sim_over_lock);
	pthread_mutex_destroy(&data->print_lock);
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i++]);
	free(data->forks);
	free(data->philos);
	free(data);
}
