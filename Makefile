# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: vbenneko <vbenneko@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/01 13:14:09 by vbenneko      #+#    #+#                  #
#    Updated: 2022/10/14 13:03:18 by vbenneko      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = philosophers
CC = gcc
NORMFLAGS = -Wall -Werror -Wextra
CFILES =
HEADERS = philosophers.h libft/libft.h

INCLUDES = $(addprefix -I, $(sort $(dir $(HEADERS))))
OBJDIR = obj
OBJFILES = $(addprefix $(OBJDIR)/,$(CFILES:c=o))
LIBFT_PATH = libft/libft.a
LIB_FLAGS = -L $(dir $(LIBFT_PATH)) -l ft

all: $(NAME)

$(NAME): $(LIBFT_PATH) $(OBJFILES)
	@$(CC) $(NORMFLAGS) $(OBJFILES) $(LIB_FLAGS) -o $(NAME)
	@printf "Compiled %s\n" "$(NAME)"

$(LIBFT_PATH):
	@git submodule update --init --recursive
	@$(MAKE) -C $(dir $(LIBFT_PATH))

$(OBJDIR)/%.o : %.c $(HEADERS) $(LIBFT_PATH)
	@mkdir -p $(@D)
	@$(call fastprint_compilation,$(CC) $(NORMFLAGS) $(INCLUDES) -c $< -o $@)

clean:
	@$(MAKE) -C $(dir $(LIBFT_PATH)) fclean
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
