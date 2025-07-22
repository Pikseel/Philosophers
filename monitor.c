/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:22:49 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 16:45:24 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static int	check_eat_limit(t_philo_info *pi)
{
	int	i;
	int	finished_count;

	if (pi->eat_limit == -1)
		return (0);
	finished_count = 0;
	pthread_mutex_lock(&pi->check_mutex);
	i = 0;
	while (i < pi->philo_size)
	{
		if (pi->philos[i].meals_eaten >= pi->eat_limit)
			finished_count++;
		i++;
	}
	pthread_mutex_unlock(&pi->check_mutex);
	if (finished_count == pi->philo_size)
	{
		pthread_mutex_lock(&pi->stop_mutex);
		pi->stop = 1;
		pthread_mutex_unlock(&pi->stop_mutex);
		return (1);
	}
	return (0);
}

static int	check_death(t_philo_info *pi, int i)
{
	unsigned long	last_eat;
	unsigned long	current_time;

	pthread_mutex_lock(&pi->check_mutex);
	last_eat = pi->philos[i].last_eat_time;
	current_time = get_ms(pi);
	pthread_mutex_unlock(&pi->check_mutex);
	if (current_time - last_eat >= pi->die_time)
	{
		pthread_mutex_lock(&pi->dead_mutex);
		printf("%lu %d died\n", get_ms(pi), i + 1);
		pthread_mutex_unlock(&pi->dead_mutex);
		pthread_mutex_lock(&pi->stop_mutex);
		pi->stop = 1;
		pthread_mutex_unlock(&pi->stop_mutex);
		return (1);
	}
	return (0);
}

void	monitor(t_philo_info *pi)
{
	int	i;

	while (1)
	{
		if (check_eat_limit(pi))
			return ;
		i = 0;
		while (i < pi->philo_size)
		{
			if (check_death(pi, i))
				return ;
			i++;
		}
		usleep(1000);
	}
}

int	arg_check(char **av)
{
	int	i;

	i = -1;
	if (ft_atoi(av[0]) == 0)
		return (-1);
	if (av[4] && ft_atoi(av[4]) == 0)
		return (-1);
	while (av[++i])
		if (ft_atoi(av[i]) == -1)
			return (-1);
	return (0);
}
