/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:20:51 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/29 18:10:39 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define SET 0
# define GET 1
# define DESTROY 0
# define JOIN 1

# include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				eat_count;
	struct s_table	*table;
	pthread_t		thread;
	size_t			tv_last_eat;
	pthread_mutex_t	m_eat;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
}	t_philo;

typedef struct s_table
{
	int				status;
	int				is_dead;
	int				philo_size;
	int				eat_limit;
	int				die_time;
	int				sleep_time;
	int				eat_time;
	size_t			tv_start;
	pthread_mutex_t	m_check;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_print;
	pthread_mutex_t	*m_forks;
	t_philo			*philos;
}	t_table;

int		create_threads(t_table *table);

int		table_status(t_table *table, int status, int flag);
void	*monitor(void *arg);

void	*philo_loop(void *arg);

int		can_eat(t_philo *p);
void	print_message(char *str, t_philo *philo, int id);
int		ft_atoi(char *str);
int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);

#endif
