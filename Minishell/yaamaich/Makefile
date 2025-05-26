NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I libft/include -lreadline -o minishell.a


LIBFT = libft/libft.a 

SRC = Main.c minishell_use.c tools.c

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

