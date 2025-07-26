CC = cc
NAME = philo
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -pthread
SRCS = philo.c monitor.c philo_loop.c utils.c
HEADERS = philo.h

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re