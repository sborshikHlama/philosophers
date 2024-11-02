TARGET = libft.a

files = ft_strlen ft_atoi ft_substr ft_strjoin \
ft_strtrim ft_split ft_bzero ft_isalnum \
ft_isalpha ft_isascii ft_isdigit ft_isprint \
ft_memcpy ft_memmove ft_memset \
ft_strlcpy ft_striteri ft_memchr ft_strrchr \
ft_strchr ft_tolower ft_toupper ft_strncmp \
ft_strlcat ft_strmapi ft_memcmp ft_calloc \
ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd

CC = gcc
CFLAGS = -Wall -Wextra -Werror

OUTPUT = $(TARGET)

CFILES = $(files:%=%.c)

OFILES = $(files:%=%.o)

NAME = $(OUTPUT)

$(NAME):
	$(CC) $(CFLAGS) -c $(CFILES) -I./
	ar -rc $(OUTPUT) $(OFILES)

all: $(NAME)

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re