NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g  # Add -g for debugging
SRC = main.cpp Irc.cpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
