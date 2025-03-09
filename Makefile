# Colors for output
END			:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
MAGENTA		:= \033[1;35m
CYAN		:= \033[1;36m
WHITE		:= \033[1;37m

# Compiler settings
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++11

# Directories
SRCDIRS		:= channel client cmd epoll server
OBJSDIR		:= objs
TSTDIR		:= test

# Find source files
SRCS		:= $(shell find $(SRCDIRS) -name '*.cpp')
OBJS		:= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))

# Executable
NAME		:= ircserv

# Test files
TSTSRC		:= $(shell find $(TSTDIR) -name '*.cpp')
TSTOBJ		:= $(patsubst ./%, $(OBJSDIR)/%, $(TSTSRC:.cpp=.o))
TSTFLAGS	:= -lgtest -lgtest_main -pthread

# Build all
all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "$(BLUE)"
	@echo "$(NAME) successfully compiled!"
	@echo "$(END)"

# Compile object files
$(OBJSDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@rm -rf $(OBJSDIR)

fclean: clean
	@rm -f $(NAME)
	@echo "$(BLUE)"
	@echo "$(NAME) fully cleaned!"
	@echo "$(END)"

re: fclean all

# Test compilation
test: $(TSTOBJ)
	@$(CXX) $(CXXFLAGS) $(TSTFLAGS) $^ -o $(TSTDIR)/test
	@echo "$(BLUE)"
	@echo "Tests compiled successfully!"
	@echo "$(END)"

# Clean test files
testclean:
	@rm -f $(TSTOBJ)

.PHONY: all clean fclean re test testclean
