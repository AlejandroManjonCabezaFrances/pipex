# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/17 09:03:02 by amanjon-          #+#    #+#              #
#    Updated: 2024/01/31 12:26:59 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I /libft/ft_printf/include -I /libft/Gnl/include -I /libft/Libft/include
DEBUG = -g3 -fsanitize=address
RM = rm -f
SRC = main.c command.c

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

SRC_DIR = ./src/
OBJ_DIR = ./obj/

OBJ_FILES = $(SRC:.c=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

##########RULES##########
all: $(OBJ_DIR) $(LIBFT_DIR) $(NAME)

$(LIBFT): $(LIBFT_DIR)
		@make -C $(LIBFT_DIR)

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o:$(SRC_DIR)%.c
		$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

##########basic library compiled##########
$(NAME): $(OBJ) $(LIBFT)
		$(CC) $(CFLAGS) $(DEBUG) $(OBJ) $(LIBFT) -o $(NAME)

##########all .o files removed##########
clean:
		$(RM) $(OBJ)
		make clean -C libft
		rm -rf $(OBJ_DIR)

##########library .a file removed##########
fclean: clean
		$(RM) $(NAME)
		make fclean -C libft
		rm -rf $(OBJ_DIR)

re:	fclean all

.PHONY:	all clean fclean re