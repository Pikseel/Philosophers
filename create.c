/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:25:32 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 18:19:47 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static int	init_forks(t_table *table, int i)
{
	if (pthread_mutex_init(&table->m_forks[i], NULL) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&table->m_forks[i]);
		pthread_mutex_destroy(&table->m_check);
		pthread_mutex_destroy(&table->m_dead);
		pthread_mutex_destroy(&table->m_print);
		return (0);
	}
	return (1);
}

static int	init_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->m_check, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&table->m_dead, NULL) != 0)
	{
		pthread_mutex_destroy(&table->m_check);
		return (0);
	}
	if (pthread_mutex_init(&table->m_print, NULL) != 0)
	{
		pthread_mutex_destroy(&table->m_check);
		pthread_mutex_destroy(&table->m_dead);
		return (0);
	}
	i = -1;
	while (++i < table->philo_size)
	{
		if (!init_forks(table, i))
			return (0);
	}
	return (1);
}

static void	destroy_mutexes(t_table *table, int thread_count, int flag)
{
	int	i;

	if (flag == JOIN)
	{
		i = -1;
		while (++i < thread_count)
			pthread_join(table->philos[i].thread, NULL);
	}
	pthread_mutex_destroy(&table->m_check);
	pthread_mutex_destroy(&table->m_dead);
	pthread_mutex_destroy(&table->m_print);
	i = -1;
	while (++i < table->philo_size)
		pthread_mutex_destroy(&table->m_forks[i]);
	i = -1;
	while (++i < thread_count)
		pthread_mutex_destroy(&table->philos[i].m_eat);
}

static int	init_philo_data(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_size)
	{
		table->philos[i].table = table;
		table->philos[i].id = i;
		table->philos[i].eat_count = 0;
		table->philos[i].tv_last_eat = get_current_time();
		table->philos[i].right_fork = &table->m_forks[(i + 1)
			% table->philo_size];
		table->philos[i].left_fork = &table->m_forks[i];
		if (pthread_mutex_init(&table->philos[i].m_eat, NULL) != 0)
		{
			destroy_mutexes(table, i, DESTROY);
			return (0);
		}
	}
	return (1);
}

int	create_threads(t_table *table)
{
	pthread_t	monitor_thread;
	int			i;

	if (init_mutexes(table) == 0)
		return (0);
	if (!init_philo_data(table))
		return (0);
	i = -1;
	while (++i < table->philo_size)
	{
		if (pthread_create(&table->philos[i].thread,
				NULL, philo_loop, (void *)&table->philos[i]))
			return (destroy_mutexes(table, i, JOIN), 0);
	}
	if (pthread_create(&monitor_thread, NULL, monitor, (void *)table))
		return (destroy_mutexes(table, i, JOIN), 0);
	if (pthread_join(monitor_thread, NULL))
	{
		destroy_mutexes(table, i, JOIN);
		return (0);
	}
	destroy_mutexes(table, i, JOIN);
	return (1);
}
