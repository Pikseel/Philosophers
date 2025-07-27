/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:25:32 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/27 15:06:17 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

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

static void	take_forks(t_philo *p)
{
	int	left_fork;
	int	right_fork;

	left_fork = p->index;
	right_fork = (p->index + 1) % p->pi->philo_size;
	if (p->index % 2 == 0)
	{
		pthread_mutex_lock(&p->pi->forks[right_fork]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->pi->forks[left_fork]);
		print_status(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->pi->forks[left_fork]);
		print_status(p, "has taken a fork");
		pthread_mutex_lock(&p->pi->forks[right_fork]);
		print_status(p, "has taken a fork");
	}
}

static void	eat_and_release(t_philo *p)
{
	int	left_fork;
	int	right_fork;
	int	first_fork;
	int	second_fork;

	left_fork = p->index;
	right_fork = (p->index + 1) % p->pi->philo_size;
	if (p->index % 2 == 0)
	{
		first_fork = right_fork;
		second_fork = left_fork;
	}
	else
	{
		first_fork = left_fork;
		second_fork = right_fork;
	}
	print_status(p, "is eating");
	eat_status(p);
	ms_sleep(p, p->pi->eat_time);
	print_status(p, "is sleeping");
	pthread_mutex_unlock(&p->pi->forks[second_fork]);
	pthread_mutex_unlock(&p->pi->forks[first_fork]);
	ms_sleep(p, p->pi->sleep_time);
	print_status(p, "is thinking");
}

static int	should_continue_eating(t_philo *p)
{
	pthread_mutex_lock(&p->pi->stop_mutex);
	if (p->pi->stop)
	{
		pthread_mutex_unlock(&p->pi->stop_mutex);
		return (0);
	}
	pthread_mutex_unlock(&p->pi->stop_mutex);
	if (p->pi->eat_limit != -1)
	{
		pthread_mutex_lock(&p->pi->check_mutex);
		if (p->meals_eaten >= p->pi->eat_limit)
		{
			pthread_mutex_unlock(&p->pi->check_mutex);
			return (0);
		}
		pthread_mutex_unlock(&p->pi->check_mutex);
	}
	return (1);
}

void	*philo_loop(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if (p->pi->philo_size == 1)
	{
		pthread_mutex_lock(&p->pi->forks[p->index]);
		print_status(p, "has taken a fork");
		ms_sleep(p, p->pi->die_time);
		pthread_mutex_unlock(&p->pi->forks[p->index]);
		return (NULL);
	}
	while (should_continue_eating(p))
	{
		if (!can_take_forks(p))
			continue ;
		take_forks(p);
		eat_and_release(p);
	}
	return (NULL);
}
