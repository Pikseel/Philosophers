CC		=	cc
NAME	=	philo
CFLAGS	=	-Wall -Wextra -Werror

SRCS	=	create.c \
			main.c \
			monitor.c \
			philo.c \
			utils.c

HEADER	=	philo.h

OBJS	=	$(SRCS:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:	%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re