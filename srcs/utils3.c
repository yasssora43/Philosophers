/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:26:00 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:45:08 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	all_philos_finished(t_data *data)
{
	int			i;
	int			finished;
	int			meals_eaten;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	finished = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meals_eaten_lock);
		meals_eaten = data->philos[i].meals_eaten;
		pthread_mutex_unlock(&data->philos[i].meals_eaten_lock);
		if (meals_eaten >= data->must_eat_count)
			finished++;
		i++;
	}
	if (finished == data->num_philos)
	{
		pthread_mutex_lock(&data->sim_over_lock);
		data->sim_over = 1;
		pthread_mutex_unlock(&data->sim_over_lock);
		return (1);
	}
	return (0);
}

int	check_simulation_over(t_philo *philos)
{
	int	sim_status;

	sim_status = 0;
	pthread_mutex_lock(&philos->data->sim_over_lock);
	sim_status = philos->data->sim_over;
	pthread_mutex_unlock(&philos->data->sim_over_lock);
	return (sim_status);
}

int	allocate_memory(t_data *data)
{
	if (data->num_philos > 300)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		return (1);
	}
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			write(2, "Error initializing fork mutex.\n", 31);
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
