CC = gcc
CFLAGS = -std=c99 -Wall -Werror -O2 -s
LIB = -Idarm

ifneq ($(OS),Windows_NT)
	PIC_FLAGS = -fPIC
endif

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

STUFF = darmbi.so

default: $(STUFF)

%.o: %.c
	$(CC) $(CFLAGS) $(LIB) -o $@ -c $^ $(PIC_FLAGS)

darmbi.so: $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean: $(OBJ) $(STUFF)
	rm -rf $^
