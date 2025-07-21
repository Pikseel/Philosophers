/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:46 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 00:27:32 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_arg(t_philo_info *pi, char **av)
{
	pi->philo_size = ft_atoi(av[1]);
	pi->die_time = ft_atoi(av[2]);
	pi->eat_time = ft_atoi(av[3]);
	pi->sleep_time = ft_atoi(av[4]);
	if (av[5])
		pi->eat_limit = ft_atoi(av[5]);
	else
		pi->eat_limit = -1;
	pi->eat_count = 0;
	pi->philos = malloc(sizeof(t_philo) * pi->philo_size);
	if (!pi->philos)
		return ;
	pi->forks = malloc(sizeof(pthread_mutex_t) * pi->philo_size);
	if (!pi->forks)
	{
		free(pi->philos);
		return ;
	}
	gettimeofday(&pi->tv, NULL);
	pi->start_ms = (pi->tv.tv_usec / 1000 + pi->tv.tv_sec * 1000);
}

void	init_thread(t_philo_info *pi)
{
	int	i;

	i = -1;
	pthread_mutex_init(&pi->dead_mutex, NULL);
	pthread_mutex_init(&pi->check_mutex, NULL);
	pthread_mutex_init(&pi->stop_mutex, NULL);
	pi->stop = 0;
	while (++i < pi->philo_size)
		pthread_mutex_init(&pi->forks[i], NULL);
	i = -1;
	while (++i < pi->philo_size)
	{
		pi->philos[i].pi = pi;
		pi->philos[i].last_eat_time = get_ms(pi);
		pi->philos[i].index = i;
		if (pthread_create(&pi->philos[i].thread, NULL, philo_loop,
				&pi->philos[i]) != 0)
			printf("Error.\n");
		usleep(500);
	}
}

static void	destroy_and_free(t_philo_info *pi)
{
	int	i;

	i = -1;
	while (++i < pi->philo_size)
		pthread_join(pi->philos[i].thread, NULL);
	pthread_mutex_destroy(&pi->dead_mutex);
	pthread_mutex_destroy(&pi->check_mutex);
	pthread_mutex_destroy(&pi->stop_mutex);
	i = -1;
	while (++i < pi->philo_size)
		pthread_mutex_destroy(&pi->forks[i]);
	free(pi->philos);
	free(pi->forks);
	free(pi);
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
	{
		free(pi);
		return (1);
	}
	init_thread(pi);
	monitor(pi);
	destroy_and_free(pi);
	return (0);
}
