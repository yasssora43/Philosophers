/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:57:05 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:51:34 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_locks_of_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->philos[i].meals_eaten_lock, NULL) != 0)
			break ;
		if (pthread_mutex_init(&data->philos[i].last_meal_time_lock, NULL) != 0)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			break ;
		}
		i++;
	}
	if (i < data->num_philos)
	{
		while (--i >= 0)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
		}
		return (1);
	}
	return (0);
}

int	init_locks_of_data(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
		}
		return (1);
	}
	if (pthread_mutex_init(&data->sim_over_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_lock);
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
		}
		return (1);
	}
	return (0);
}

int	init_locks(t_data *data)
{
	int	i;

	if (init_locks_of_philo(data))
		return (1);
	if (init_locks_of_data(data))
		return (1);
	if (pthread_mutex_init(&data->start_time_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->sim_over_lock);
		pthread_mutex_destroy(&data->print_lock);
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
		}
		return (1);
	}
	return (0);
}

void	init_philos(t_data *data)
{
	int	i;

	data->sim_over = 0;
	data->start_time = get_time_ms();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eating = 0;
		data->philos[i].meals_eaten = 0;
		data->philos[i].left_fork = i;
		if (i == 0)
			data->philos[i].right_fork = data->num_philos - 1;
		else
			data->philos[i].right_fork = (i - 1);
		data->philos[i].data = data;
		data->philos[i].last_meal_time = get_time_ms();
		i++;
	}
}

int	init_mutexes_and_data(t_data *data)
{
	int	i;

	if (allocate_memory(data))
		return (1);
	if (init_locks(data))
		return (1);
	if (init_forks(data))
	{
		pthread_mutex_destroy(&data->start_time_lock);
		pthread_mutex_destroy(&data->sim_over_lock);
		pthread_mutex_destroy(&data->print_lock);
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meals_eaten_lock);
			pthread_mutex_destroy(&data->philos[i].last_meal_time_lock);
			i++;
		}
		free(data->philos);
		free(data->forks);
		return (1);
	}
	init_philos(data);
	return (0);
}
