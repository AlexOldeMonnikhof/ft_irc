NAME = ircserv
SRC = main.cpp test.cpp
VPATH = ./src
FLAGS = -Wall -Wextra -Werror -std=c++98 -I inc
CC = c++
HEADERS = inc/test.hpp

# OBJ = $(SRC:.cpp=.o)
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp $(HEADERS) | $(OBJ_DIR)
	$(CC) $(FLAGS) -o $@ -c $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ) $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all