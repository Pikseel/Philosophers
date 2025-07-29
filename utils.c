/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:20:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 17:25:34 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int	can_eat(t_philo *p)
{
	t_table	*table;

	table = p->table;
	pthread_mutex_lock(&table->m_dead);
	if (table->is_dead)
	{
		pthread_mutex_unlock(&table->m_dead);
		return (0);
	}
	pthread_mutex_unlock(&table->m_dead);
	return (1);
}

void	print_message(char *str, t_philo *philo, int id)
{
	size_t	time;

	if (!str || !philo || !philo->table)
		return ;
	pthread_mutex_lock(&philo->table->m_print);
	time = get_current_time() - philo->table->tv_start;
	if (can_eat(philo))
		printf("%zu %d %s\n", time, id + 1, str);
	pthread_mutex_unlock(&philo->table->m_print);
}

int	ft_atoi(char *str)
{
	int		i;
	long	res;

	if (!str)
		return (-1);
	i = 0;
	res = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	if (!(str[i] >= '0' && str[i] <= '9') && str[i] != '\0')
		return (-1);
	return ((int)res);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
