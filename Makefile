# Compiler and flags
CC=gcc
INCDIRS=-I. -I./libs/libft
CFLAGS=-Wall -Wextra -g

# Source files
CFILES=main.c init.c dinner.c parse.c safe_functions.c synchro.c getters_setters.c utils.c write.c monitor.c

# Paths
OBJDIR=objects
OBJECTS=$(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))

# Paths for libft
LIBFT_DIR=./libs/libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a

# Output binary
BINARY=philo

# Default target
all: $(LIBFT_LIB) $(BINARY)

# Link the binary
$(BINARY): $(OBJECTS) $(LIBFT_LIB)
	$(CC) -o $@ $^ -L$(LIBFT_DIR) -lft

# Compile object files to the objects folder
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# Create the objects folder if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Make sure libft is compiled
$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

# Clean all objects and binaries
clean:
	rm -rf $(BINARY) $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean
