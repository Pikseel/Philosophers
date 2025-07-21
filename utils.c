/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:20:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 00:27:32 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *p, char *str)
{
	pthread_mutex_lock(&p->pi->stop_mutex);
	if (p->pi->stop)
	{
		pthread_mutex_unlock(&p->pi->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&p->pi->stop_mutex);
	pthread_mutex_lock(&p->pi->dead_mutex);
	printf("%lu %d %s\n", get_ms(p->pi), p->index + 1, str);
	pthread_mutex_unlock(&p->pi->dead_mutex);
}

void	eat_status(t_philo *p)
{
	pthread_mutex_lock(&p->pi->check_mutex);
	p->pi->eat_count++;
	p->last_eat_time = get_ms(p->pi);
	pthread_mutex_unlock(&p->pi->check_mutex);
}

unsigned long	get_ms(t_philo_info *pi)
{
	gettimeofday(&pi->tv, NULL);
	return ((pi->tv.tv_usec / 1000 + pi->tv.tv_sec * 1000) - pi->start_ms);
}

void	ms_sleep(t_philo *p, int time)
{
	unsigned long	wakeup_time;

	wakeup_time = get_ms(p->pi) + time;
	while (get_ms(p->pi) < wakeup_time)
		usleep(500);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] == ' ' || str[i] == '\t')
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
	return (res);
}
