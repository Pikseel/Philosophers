/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:25:32 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 00:22:38 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks_and_eat(t_philo *p)
{
	pthread_mutex_lock(&p->pi->forks[p->index]);
	print_status(p, "has taken a fork");
	pthread_mutex_lock(&p->pi->forks[(p->index + 1) % p->pi->philo_size]);
	print_status(p, "has taken a fork");
	print_status(p, "is eating");
	eat_status(p);
	ms_sleep(p, p->pi->eat_time);
	print_status(p, "is sleeping");
	pthread_mutex_unlock(&p->pi->forks[p->index]);
	pthread_mutex_unlock(&p->pi->forks[(p->index + 1) % p->pi->philo_size]);
	ms_sleep(p, p->pi->sleep_time);
	print_status(p, "is thinking");
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
	while (1)
	{
		pthread_mutex_lock(&p->pi->stop_mutex);
		if (p->pi->stop)
		{
			pthread_mutex_unlock(&p->pi->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&p->pi->stop_mutex);
		take_forks_and_eat(p);
	}
	return (NULL);
}
