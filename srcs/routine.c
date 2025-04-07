/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:32:40 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:23:00 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	precise_sleep(time_t time_to_sleep)
{
	time_t	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_to_sleep)
	{
		usleep(50);
	}
}

void	eat(t_philo *philos)
{
	long long	now;

	pthread_mutex_lock(&philos->data->forks[philos->right_fork]);
	print_status(philos, "has taken a fork");
	pthread_mutex_lock(&philos->data->forks[philos->left_fork]);
	print_status(philos, "has taken a fork");
	philos->eating = 1;
	now = get_time_ms();
	pthread_mutex_lock(&philos->last_meal_time_lock);
	philos->last_meal_time = now;
	pthread_mutex_unlock(&philos->last_meal_time_lock);
	print_status(philos, "is eating");
	pthread_mutex_lock(&philos->meals_eaten_lock);
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->meals_eaten_lock);
	precise_sleep(philos->data->time_to_eat);
	philos->eating = 0;
	pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
	pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
}

void	sleep_think(t_philo *philos)
{
	print_status(philos, "is sleeping");
	precise_sleep(philos->data->time_to_sleep);
	print_status(philos, "is thinking");
}

void	*routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *)arg;
	if (philos->data->must_eat_count == 0)
		return (NULL);
	if (philos->data->num_philos == 1)
	{
		pthread_mutex_lock(&philos->data->forks[philos->right_fork]);
		print_status(philos, "has taken a fork");
		precise_sleep(philos->data->time_to_die);
		pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
		return (NULL);
	}
	if (philos->id % 2 == 0)
		precise_sleep(10);
	while (!check_simulation_over(philos))
	{
		eat(philos);
		sleep_think(philos);
	}
	return (NULL);
}
