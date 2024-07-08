# Define directories for source and include files
SRCDIR = src
INCDIR = include

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

# Target and object files
TARGET = my_tar
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DEPS = $(wildcard $(INCDIR)/*.h)

# Rules to build objects from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Target to build the final executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Create the obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean rules
clean:
	rm -f $(OBJDIR)/*.o

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)