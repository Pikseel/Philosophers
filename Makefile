CC = @cc
NAME = philo
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread
SRCS = philo.c monitor.c philo_loop.c utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)
	
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re