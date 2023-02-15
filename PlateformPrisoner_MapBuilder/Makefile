#by HAYAT Rahim


CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -lncurses
EXEC = prog 
SRC= $(wildcard *.c) $(wildcard */*.c)
OBJ = $(SRC:.c=.o)

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm -f $(EXEC) $(OBJ)


