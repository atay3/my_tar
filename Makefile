CC=gcc
CFLAGS=-Wall -Wextra -Werror
DEPS = my_tar.h
TARGET = my_tar
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_tar: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)