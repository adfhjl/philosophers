# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: vbenneko <vbenneko@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/01 13:14:09 by vbenneko      #+#    #+#                  #
#    Updated: 2022/10/26 16:27:36 by vbenneko      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = gcc
NORMFLAGS = -Wall -Werror -Wextra
CFILES =					\
	src/main.c				\
	src/error.c				\
	src/init_philos.c		\
	src/philo_utils.c		\
	src/thread_func.c		\
	src/init_mutexes.c		\
	src/philo_actions.c		\
	src/monitor_philos.c	\
	src/ft_atost_strict.c	\
	src/destroy_mutexes.c
HEADERS = philosophers.h

INCLUDES = $(addprefix -I, $(sort $(dir $(HEADERS))))
OBJDIR = obj
OBJFILES = $(addprefix $(OBJDIR)/,$(CFILES:c=o))
LIB_FLAGS = -l pthread

all: $(NAME)

$(NAME): $(OBJFILES)
	@$(CC) $(NORMFLAGS) $(OBJFILES) $(LIB_FLAGS) -o $(NAME)
	@printf "Compiled %s\n" "$(NAME)"

$(OBJDIR)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	@$(call fastprint_compilation,$(CC) $(NORMFLAGS) $(INCLUDES) -c $< -o $@)

clean:
	@rm -rf $(OBJDIR)
	@printf "Cleaned %s\n" "$(NAME)"

fclean: clean
	@rm -f $(NAME)
	@printf "Deleted %s\n" "$(NAME)"

re: fclean all

define fastprint_compilation
	@printf "%s\e[K\n" "$(1)"
	@$(1)
	@printf "\e[A\e[K"
endef

.PHONY: all clean fclean re
