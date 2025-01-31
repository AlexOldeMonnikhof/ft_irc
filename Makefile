NAME = ircserv
SRC = 	main.cpp Server.cpp Client.cpp Command.cpp Channel.cpp utils.cpp \
		invite.cpp join.cpp kick.cpp mode.cpp nick.cpp part.cpp pass.cpp privmsg.cpp topic.cpp user.cpp op.cpp limit.cpp key.cpp quit.cpp Chatbot.cpp
VPATH = src inc src/cmd
FLAGS = -Wall -Wextra -Werror -std=c++98 -I inc
CC = c++
HEADERS = Channel.hpp Chatbot.hpp Client.hpp Command.hpp Server.hpp error.hpp headers.hpp

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
