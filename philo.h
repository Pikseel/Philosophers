/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:51 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 00:27:32 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

struct	s_philo_info;

typedef struct s_philo
{
	struct s_philo_info	*pi;
	pthread_t			thread;
	int					index;
	unsigned long		last_eat_time;
}	t_philo;

typedef struct s_philo_info
{
	int				philo_size;
	unsigned long	die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_limit;
	int				eat_count;
	unsigned long	start_ms;
	struct timeval	tv;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	check_mutex;
	int				stop;
	pthread_mutex_t	stop_mutex;
}	t_philo_info;

unsigned long	get_ms(t_philo_info *pi);
void			print_status(t_philo *p, char *str);
void			eat_status(t_philo *p);
void			*philo_loop(void *philo);
void			init_arg(t_philo_info *pi, char **av);
void			init_thread(t_philo_info *pi);
void			monitor(t_philo_info *pi);
void			ms_sleep(t_philo *p, int time);
int				ft_atoi(char *str);
int				arg_check(char **av);

#endif
