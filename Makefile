NAME := ircserv

VPATH := src inc src/cmd
SRC := Channel.cpp Chatbot.cpp Client.cpp Command.cpp main.cpp Server.cpp utils.cpp \
	   invite.cpp join.cpp key.cpp kick.cpp limit.cpp mode.cpp nick.cpp op.cpp part.cpp pass.cpp privmsg.cpp quit.cpp topic.cpp user.cpp
HEADERS := Channel.hpp Chatbot.hpp Client.hpp Command.hpp error.hpp Server.hpp

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -I inc -g3
DEP_FLAGS := -MMD -MP

OBJ_DIR := obj
OBJ := $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS := $(OBJ:.o=.d)

# Colors for help message
BLUE := \033[36m
NC := \033[0m

.PHONY: all
all: $(NAME)  ## Build the irc server

.PHONY: clean
clean:  ## Remove object dir
	rm -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean  ## Full clean up
	rm -rf $(NAME)

.PHONY: re
re: fclean all  ## Rebuild server from scratch

.PHONY: help
help:  ## Get help
	@echo -e 'Usage: make ${BLUE}<target>${NC}'
	@echo -e 'Available targets:'
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "  ${BLUE}%-15s${NC} %s\n", $$1, $$2}' $(MAKEFILE_LIST)

# compile the server
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

# create object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEP_FLAGS) -o $@ -c $<

# create object dir
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

-include $(DEPS)
