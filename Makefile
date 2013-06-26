CC = gcc
CFLAGS = -std=c99 -Wall -Werror -O2 -s

EXLIB = -Idarm
EXOBJ = -Ldarm

ifneq ($(OS),Windows_NT)
	PIC_FLAGS = -fPIC
endif

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

STUFF = darmbi.so tests.exe

default: $(STUFF)

test: darmbi.so tests.exe
	./tests.exe

%.o: %.c
	$(CC) $(CFLAGS) $(EXLIB) -o $@ -c $^ $(PIC_FLAGS)

darmbi.so: $(OBJ)
	$(CC) $(CFLAGS) $(EXOBJ) -shared -o $@ $^ -ldarm

tests.exe: tests.c darmbi.so
	$(CC) $(CFLAGS) -o $@ tests.c darmbi.so

clean:
	rm -f $(OBJ) $(STUFF)
