# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/14 17:15:51 by aevstign          #+#    #+#              #
#    Updated: 2024/12/28 11:15:39 by aevstign         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# colors
RED		=	"\033[0;31m"
GREEN	=	"\033[0;32m"
RESET	=	"\033[0m"

NAME	=	philo

#comands
CC	=	cc
DFLAGS	=	-g #-g3 -gdwarf-3 -fsanitize=thread
CFLAGS	=	-fsanitize=thread
RM		=	rm -rf
SILENT	=	--silent

#directories
PHILO_DIR	=	./
BUILD_DIR	=	./objects/

#files
PHILO_SOURCES	=	$(PHILO_DIR)/simulation.c \
					$(PHILO_DIR)/init.c \
					$(PHILO_DIR)/parse.c \
					$(PHILO_DIR)/synchro.c \
					$(PHILO_DIR)/getters_setters.c \
					$(PHILO_DIR)/monitor.c \
					$(PHILO_DIR)/main.c \
					$(PHILO_DIR)/utils.c \
					$(PHILO_DIR)/write.c \

#build
PHILO_OBJS	=	$(PHILO_SOURCES:$(PHILO_DIR)%.c=$(BUILD_DIR)%.o)

#rules
all: $(NAME)

$(NAME): $(PHILO_OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(PHILO_OBJS) $(DFLAGS)
	@echo $(GREEN)"\n✅✅✅✅✅✅✅✅✅✅✅✅\n✅✅✅Bon appetit!✅✅✅\n✅✅✅✅✅✅✅✅✅✅✅✅\n"$(RESET)

$(BUILD_DIR)%.o: $(PHILO_DIR)%.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@
	@echo $(GREEN)"✅ $<"$(RESET)

clean:
	@$(RM) $(BUILD_DIR)
	@echo $(RED)"❗️ cleaned"$(RESET)

fclean: clean
	@$(RM) $(NAME)
	@echo $(RED)"❗️ fcleaned"$(RESET)
	@echo $(RED)"\n❗️❗️❗️❗️❗️❗️❗️❗️❗️\n❗️❗️❗️PHILO!❗️❗️❗️\n❗️❗️❗️❗️❗️❗️❗️❗️❗️\n"$(RESET)


re: fclean all

.PHONY: all clean fclean re