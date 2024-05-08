CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEPS = main.h archive.h
TARGET = my_tar
OBJ = main.o utils.o archive.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)