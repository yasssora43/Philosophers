NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -pthread
SRCS = srcs/main.c \
		srcs/init.c \
		srcs/routine.c \
		srcs/utils1.c \
		srcs/utils2.c \
		srcs/utils3.c \

OBJCS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME): $(OBJCS)
		$(CC) $(CFLAGS) $(OBJCS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS)  -o $@
clean:
	rm -rf $(OBJCS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJCS)