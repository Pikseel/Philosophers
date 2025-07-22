/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:51 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/22 16:50:50 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

struct	s_philo_info;

typedef struct s_philo
{
	struct s_philo_info	*pi;
	pthread_t			thread;
	int					index;
	unsigned long		last_eat_time;
	int					meals_eaten;
}	t_philo;

typedef struct s_philo_info
{
	int				philo_size;
	unsigned long	die_time;
	int				eat_time;
	int				sleep_time;
	int				eat_limit;
	unsigned long	start_ms;
	struct timeval	tv;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	check_mutex;
	int				stop;
	pthread_mutex_t	stop_mutex;
}	t_philo_info;

void			destroy_mutexes(t_philo_info *pi, int thread_count);
int				arg_check(char **av);
void			monitor(t_philo_info *pi);
void			destroy_and_free(t_philo_info *pi);
void			*philo_loop(void *philo);
void			print_status(t_philo *p, char *str);
void			eat_status(t_philo *p);
unsigned long	get_ms(t_philo_info *pi);
void			ms_sleep(t_philo *p, int time);
int				ft_atoi(char *str);

#endif
