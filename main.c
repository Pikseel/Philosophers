/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:05:48 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 18:01:59 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

static int	is_zero(t_table *pi)
{
	if (pi->die_time == 0 || pi->eat_time == 0 || pi->sleep_time == 0
		|| pi->philo_size == 0)
	{
		printf("Error: Time values cant be zero.\n");
		return (1);
	}
	return (0);
}

static int	init_data(t_table *table, int argc, char **argv)
{
	table->philo_size = ft_atoi(argv[1]);
	table->die_time = ft_atoi(argv[2]);
	table->eat_time = ft_atoi(argv[3]);
	table->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		table->eat_limit = ft_atoi(argv[5]);
	else
		table->eat_limit = -1;
	table->m_forks = NULL;
	table->philos = NULL;
	table->status = 0;
	table->is_dead = 0;
	if (is_zero(table))
		return (0);
	table->m_forks = malloc(table->philo_size * sizeof(pthread_mutex_t));
	if (!table->m_forks)
		return (0);
	table->philos = malloc(sizeof(t_philo) * table->philo_size);
	if (!table->philos)
	{
		free(table->m_forks);
		return (0);
	}
	return (1);
}

static int	arg_check(char **av)
{
	int	i;

	i = 0;
	if (ft_atoi(av[0]) <= 0)
		return (-1);
	while (av[i])
	{
		if (ft_atoi(av[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	if ((ac != 5 && ac != 6) || arg_check(av + 1) == -1)
	{
		printf("Error: Invalid argument.\n");
		return (1);
	}
	if (!init_data(&table, ac, av))
	{
		printf("Error: init_data\n");
		if (table.philos)
			free(table.philos);
		if (table.m_forks)
			free(table.m_forks);
		return (1);
	}
	if (!create_threads(&table))
	{
		printf("Error: create_threads\n");
		return (free(table.philos), free(table.m_forks), 1);
	}
	free(table.philos);
	free(table.m_forks);
	return (0);
}
