/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 01:18:26 by ykhindou          #+#    #+#             */
/*   Updated: 2025/04/06 08:32:07 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				eating;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	struct s_data	*data;
	pthread_mutex_t	meals_eaten_lock;
	pthread_mutex_t	last_meal_time_lock;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	int				sim_over;
	time_t			start_time;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_over_lock;
	pthread_mutex_t	start_time_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				start_flag;
}	t_data;

int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			parse_args(int argc, char *argv[]);
int			is_valid_number(char *str);
void		print_status(t_philo *philos, const char *status);
long long	get_time_ms(void);
int			allocate_memory(t_data *data);
int			init_forks(t_data *data);
void		precise_sleep(time_t time_to_sleep);
void		*routine(void *arg);
int			check_simulation_over(t_philo *philos);
void		handle_single_philo(t_philo *philos);
void		free_mutexes_and_data(t_data *data);
int			is_philo_died(t_data *data);
int			all_philos_finished(t_data *data);
int			init_mutexes_and_data(t_data *data);

#endif