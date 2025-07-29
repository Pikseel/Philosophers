/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:46 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 17:21:39 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_think(t_philo *philo)
{
	int	i;

	if (!philo || !philo->table)
		return ;
	print_message("is thinking", philo, philo->id);
	i = (philo->table->die_time - (philo->table->sleep_time
				+ philo->table->eat_time)) / 2;
	if (i < 0)
		i = 0;
	ft_usleep(i);
}

static void	philo_sleep(t_philo *philo)
{
	if (!philo || !philo->table)
		return ;
	print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->table->sleep_time);
}

static void	take_forks(t_philo *philo)
{
	if (!philo || !philo->left_fork || !philo->right_fork)
		return ;
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo, philo->id);
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork", philo, philo->id);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork", philo, philo->id);
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo, philo->id);
	}
}

static void	philo_eat(t_philo *philo)
{
	if (!philo || !philo->table)
		return ;
	if (philo->table->philo_size == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo, philo->id);
		ft_usleep(philo->table->die_time);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	take_forks(philo);
	print_message("is eating", philo, philo->id);
	ft_usleep(philo->table->eat_time);
	pthread_mutex_lock(&philo->m_eat);
	philo->tv_last_eat = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->m_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!philo || !philo->table)
		return (NULL);
	while (table_status(philo->table, 0, GET) == 0)
		;
	pthread_mutex_lock(&philo->m_eat);
	philo->tv_last_eat = get_current_time();
	pthread_mutex_unlock(&philo->m_eat);
	if (philo->id % 2)
		ft_usleep(1);
	while (can_eat(philo))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
