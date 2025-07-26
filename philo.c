/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:46 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/26 12:41:47 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void	init_arg(t_philo_info *pi, char **av)
{
	pi->philo_size = ft_atoi(av[1]);
	pi->die_time = ft_atoi(av[2]);
	pi->eat_time = ft_atoi(av[3]);
	pi->sleep_time = ft_atoi(av[4]);
	if (av[5])
		pi->eat_limit = ft_atoi(av[5]);
	else
		pi->eat_limit = -1;
	pi->philos = malloc(sizeof(t_philo) * pi->philo_size);
	if (!pi->philos)
	{
		pi->forks = NULL;
		return ;
	}
	pi->forks = malloc(sizeof(pthread_mutex_t) * pi->philo_size);
	if (!pi->forks)
	{
		free(pi->philos);
		pi->philos = NULL;
		return ;
	}
	gettimeofday(&pi->tv, NULL);
	pi->start_ms = (pi->tv.tv_usec / 1000 + pi->tv.tv_sec * 1000);
}

static int	init_mutexes(t_philo_info *pi)
{
	int	i;

	if (pthread_mutex_init(&pi->dead_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&pi->check_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&pi->dead_mutex), -1);
	if (pthread_mutex_init(&pi->stop_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&pi->dead_mutex),
			pthread_mutex_destroy(&pi->check_mutex), -1);
	i = -1;
	while (++i < pi->philo_size)
	{
		if (pthread_mutex_init(&pi->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&pi->forks[i]);
			pthread_mutex_destroy(&pi->stop_mutex);
			pthread_mutex_destroy(&pi->check_mutex);
			return (pthread_mutex_destroy(&pi->dead_mutex), -1);
		}
	}
	return (0);
}

void	destroy_mutexes(t_philo_info *pi, int thread_count)
{
	int	i;

	i = -1;
	while (++i < thread_count)
		pthread_join(pi->philos[i].thread, NULL);
	pthread_mutex_destroy(&pi->dead_mutex);
	pthread_mutex_destroy(&pi->check_mutex);
	pthread_mutex_destroy(&pi->stop_mutex);
	i = -1;
	while (++i < pi->philo_size)
		pthread_mutex_destroy(&pi->forks[i]);
}

static int	init_thread(t_philo_info *pi)
{
	int	i;

	if (init_mutexes(pi) == -1)
		return (-1);
	pi->stop = 0;
	i = -1;
	while (++i < pi->philo_size)
	{
		pi->philos[i].pi = pi;
		pi->philos[i].last_eat_time = get_ms(pi);
		pi->philos[i].index = i;
		pi->philos[i].meals_eaten = 0;
		if (pthread_create(&pi->philos[i].thread, NULL, philo_loop,
				&pi->philos[i]) != 0)
			return (destroy_mutexes(pi, i), -1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_philo_info	*pi;

	if ((ac != 5 && ac != 6) || arg_check(av + 1) == -1)
	{
		printf("Arg Error.\n");
		return (1);
	}
	pi = malloc(sizeof(t_philo_info));
	if (!pi)
		return (1);
	init_arg(pi, av);
	if (!pi->philos || !pi->forks)
		return (free(pi), 1);
	if (init_thread(pi) == -1)
	{
		free(pi->philos);
		free(pi->forks);
		free(pi);
		return (1);
	}
	monitor(pi);
	destroy_and_free(pi);
	return (0);
}
