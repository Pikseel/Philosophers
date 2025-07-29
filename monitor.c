/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:22:49 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 17:21:32 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_philo *philo, size_t die_time)
{
	if (!philo)
		return (1);
	pthread_mutex_lock(&philo->m_eat);
	if (get_current_time() - philo->tv_last_eat > die_time)
	{
		pthread_mutex_unlock(&philo->m_eat);
		return (1);
	}
	pthread_mutex_unlock(&philo->m_eat);
	return (0);
}

static int	monitor_eat(t_table *table)
{
	int	i;
	int	count;

	if (!table || !table->philos)
		return (0);
	i = 0;
	count = 0;
	if (table->eat_limit == -1)
		return (0);
	while (i < table->philo_size)
	{
		pthread_mutex_lock(&table->philos[i].m_eat);
		if (table->philos[i].eat_count >= table->eat_limit)
			count++;
		pthread_mutex_unlock(&table->philos[i].m_eat);
		i++;
	}
	if (count == table->philo_size)
	{
		pthread_mutex_lock(&table->m_dead);
		table->is_dead = 1;
		pthread_mutex_unlock(&table->m_dead);
		return (1);
	}
	return (0);
}

static int	monitor_death(t_table *table)
{
	int		i;

	if (!table || !table->philos)
		return (1);
	i = 0;
	while (i < table->philo_size)
	{
		if (check_death(&table->philos[i], table->die_time))
		{
			print_message("died", &table->philos[i], table->philos[i].id);
			pthread_mutex_lock(&table->m_dead);
			table->is_dead = 1;
			pthread_mutex_unlock(&table->m_dead);
			return (1);
		}
		i++;
	}
	return (0);
}

int	table_status(t_table *table, int status, int flag)
{
	int	ret;

	if (flag == SET)
	{
		pthread_mutex_lock(&table->m_check);
		table->status = status;
		pthread_mutex_unlock(&table->m_check);
	}
	else if (flag == GET)
	{
		pthread_mutex_lock(&table->m_check);
		ret = table->status;
		pthread_mutex_unlock(&table->m_check);
		return (ret);
	}
	return (-1);
}

void	*monitor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	if (!table)
		return (NULL);
	table->tv_start = get_current_time();
	table_status(table, 1, SET);
	while (1)
	{
		if (monitor_death(table))
			break ;
		if (monitor_eat(table))
			break ;
	}
	return (NULL);
}
