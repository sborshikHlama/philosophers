# Compiler and flags
CC=gcc
INCDIRS=-I. 
CFLAGS=-Wall -Wextra -Werror -pthread

# Source files
CFILES=main.c init.c dinner.c parse.c safe_functions.c \
		synchro.c getters_setters.c utils.c \
		write.c monitor.c

# Paths
OBJDIR=objects
OBJECTS=$(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))

# Output binary
BINARY=philo

# Default target
all: $(BINARY)

# Link the binary
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# Compile object files to the objects folder
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# Create the objects folder if it doesn't exist
$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean all objects and binaries
clean:
	rm -rf $(BINARY) $(OBJDIR)
