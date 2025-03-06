END			:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
MAGENTA		:= \033[1;35m
CYAN		:= \033[1;36m
WHITE		:= \033[1;37m

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98

OBJSDIR		:= objs
UNAME := $(shell uname)

# Set SRCS based on OS
ifeq ($(UNAME), Darwin)
    SRCS := $(shell find . -name '*.cpp')  # macOS requires the dot explicitly  # !FIX This is using wildcard and may be considered bad practice
else
    SRCS := $(shell find -name '*.cpp')    # Works on Linux
endif

OBJS		:= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))
NAME		:= ircserv

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)stuff compiled$(END)\n"

$(OBJSDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(NAME)
	@rm -rf $(OBJSDIR)

fclean: clean
	@rm -f $(NAME)
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)stuff removed$(END)\n"

re: fclean all