NAME = minishell.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I libft/include 

LIBFT = libft/libft.a 

SRC = Main.c

all: $(NAME)

$(NAME): $(LIBFT) $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

clean:
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re

