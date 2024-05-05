CC = gcc
OBJ_DIR = objs/
SRC_DIR = src/
OBJS = $(OBJ_DIR)sic.o

CFLAGS= -Wall -Wextra
PROG_NAME = sic
$(PROG_NAME) : $(OBJS)
	@$(CC) -o $(PROG_NAME) $(OBJS) $(CFLAGS)

$(OBJ_DIR)sic.o : $(SRC_DIR)sic.c
	@$(CC) $(CFLAGS) -c $(SRC_DIR)sic.c -o $(OBJ_DIR)sic.o

run:
	./$(PROG_NAME) Sooo fast, like a turle...


test_speed:
	./$(PROG_NAME) | pv > /dev/null

.PHONY : clean
clean :
	rm $(PROG_NAME) $(OBJS)
