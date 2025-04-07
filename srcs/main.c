/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 03:05:30 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:25:39 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	is_philo_died(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].last_meal_time_lock);
		if (get_time_ms() - data->philos[i].last_meal_time >= \
		data->time_to_die && data->philos[i].eating == 0)
		{
			pthread_mutex_unlock(&data->philos[i].last_meal_time_lock);
			if (!data->sim_over)
			{
				print_status(data->philos, "is died");
				pthread_mutex_lock(&data->sim_over_lock);
				data->sim_over = 1;
				pthread_mutex_unlock(&data->sim_over_lock);
			}
			return (1);
		}
		pthread_mutex_unlock(&data->philos[i].last_meal_time_lock);
		i++;
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
		if (is_philo_died(data) || all_philos_finished(data))
			break ;
	return (NULL);
}

void	create_philos(t_data *data)
{
	int			i;
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, &monitor_death, data) != 0)
		return (free_mutexes_and_data(data));
	i = -1;
	while (++i < data->num_philos)
		if (pthread_create(&data->philos[i].thread, NULL, \
		routine, &data->philos[i]) != 0)
			return (free_mutexes_and_data(data));
	i = -1;
	if (pthread_join(monitor, NULL) != 0)
		return (free_mutexes_and_data(data));
	while (++i < data->num_philos)
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (free_mutexes_and_data(data));
}

t_data	*init_table(int argc, char *argv[])
{
	t_data	*table;

	if (parse_args(argc, argv))
		return (NULL);
	table = malloc(sizeof(t_data));
	if (!table)
		return (NULL);
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->must_eat_count = ft_atoi(argv[5]);
	else
		table->must_eat_count = -1;
	table->sim_over = 0;
	return (table);
}

int	main(int argc, char *argv[])
{
	t_data	*table;

	table = init_table(argc, argv);
	if (!table)
		return (1);
	if (init_mutexes_and_data(table))
	{
		write(2, "Error initializing mutexes or memory allocation.\n", 49);
		free(table);
		return (1);
	}
	create_philos(table);
	free_mutexes_and_data(table);
	return (0);
}
