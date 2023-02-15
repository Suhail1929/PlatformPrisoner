#by HAYAT Rahim


CC = gcc
EXEC = prog 
SRC= $(wildcard *.c) $(wildcard */*.c)
OBJ = $(SRC:.c=.o)

all : $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o : %.c 
	$(CC) -o $@ -c $<

clean :
	rm -f $(EXEC) $(OBJ)