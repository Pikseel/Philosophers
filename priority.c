/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:00:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/27 15:07:22 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static int	check_starvation(t_philo *p, unsigned long curr_time,
			unsigned long my_time_since_eat)
{
	int				i;
	unsigned long	t_since_last_eat;

	i = 0;
	while (i < p->pi->philo_size)
	{
		if (i != p->index)
		{
			t_since_last_eat = curr_time - p->pi->philos[i].last_eat_time;
			if (t_since_last_eat > my_time_since_eat
				&& t_since_last_eat >= (p->pi->die_time * 0.4))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_hungry_philos(t_philo *p, unsigned long curr_time,
			int min_meals)
{
	int				i;
	unsigned long	t_since_last_eat;

	i = 0;
	while (i < p->pi->philo_size)
	{
		if (i != p->index && p->pi->philos[i].meals_eaten <= min_meals)
		{
			t_since_last_eat = curr_time - p->pi->philos[i].last_eat_time;
			if (t_since_last_eat >= (p->pi->die_time * 0.25))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_meal_balance(t_philo *p, unsigned long curr_time)
{
	int	i;
	int	min_meals;

	min_meals = p->meals_eaten;
	i = 0;
	while (i < p->pi->philo_size)
	{
		if (p->pi->philos[i].meals_eaten < min_meals)
			min_meals = p->pi->philos[i].meals_eaten;
		i++;
	}
	if (p->meals_eaten <= min_meals)
		return (0);
	if (check_hungry_philos(p, curr_time, min_meals))
		return (1);
	if ((curr_time % 60) < 10)
		return (1);
	return (0);
}

int	should_give_priority(t_philo *p, int fork_index)
{
	unsigned long	curr_time;
	unsigned long	my_time_since_eat;

	(void)fork_index;
	pthread_mutex_lock(&p->pi->check_mutex);
	curr_time = get_ms(p->pi);
	my_time_since_eat = curr_time - p->last_eat_time;
	if (my_time_since_eat >= (p->pi->die_time * 0.8))
	{
		pthread_mutex_unlock(&p->pi->check_mutex);
		return (0);
	}
	if (check_starvation(p, curr_time, my_time_since_eat))
	{
		pthread_mutex_unlock(&p->pi->check_mutex);
		return (1);
	}
	if (check_meal_balance(p, curr_time))
	{
		pthread_mutex_unlock(&p->pi->check_mutex);
		return (1);
	}
	pthread_mutex_unlock(&p->pi->check_mutex);
	return (0);
}

int	can_take_forks(t_philo *p)
{
	int	left_fork;
	int	right_fork;

	left_fork = p->index;
	right_fork = (p->index + 1) % p->pi->philo_size;
	if (should_give_priority(p, left_fork)
		|| should_give_priority(p, right_fork))
	{
		usleep(50);
		return (0);
	}
	return (1);
}
