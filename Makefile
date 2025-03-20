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
CXXFLAGS	:= -Wall -Wextra -Werror -g -std=c++98

# Directories
SRCDIRS		:= channel client cmd epoll server
OBJSDIR		:= objs

DATA_DIR	:= data
DATA_FILES	:= channels.csv clients.csv requests.csv channel_users.csv

# Find source files
SRCS		:=  cmd/userCmd.cpp \
				cmd/nickCmd.cpp \
				cmd/modeCmd.cpp \
				cmd/passCmd.cpp \
				cmd/joinCmd.cpp \
				cmd/quitCmd.cpp \
				cmd/privmsgCmd.cpp \
				cmd/topicCmd.cpp \
				cmd/partCmd.cpp \
				cmd/inviteCmd.cpp \
				cmd/infoChannel.cpp \
				cmd/kickCmd.cpp \
				epoll/EpollManager.cpp \
				server/serverData.cpp \
				server/serverNetwork.cpp \
				server/serverUtils.cpp \
				server/Irc.cpp \
				server/main.cpp \
				server/server.cpp \
				channel/ChannelInvite.cpp \
				channel/ChannelUser.cpp \
				channel/Channel.cpp \
				channel/ChannelMode.cpp \
				client/Client.cpp

OBJS		:= $(addprefix $(OBJSDIR)/, $(SRCS:.cpp=.o))

# Executable
NAME		:= ircserv

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

clean_data:
	@for file in $(DATA_FILES); do \
		> $(DATA_DIR)/$$file; \
	done

fclean: clean clean_data
	@rm -f $(NAME)
	@echo "$(BLUE)"
	@echo "$(NAME) fully cleaned!"
	@echo "$(END)"

re: fclean all

.PHONY: all clean fclean re
