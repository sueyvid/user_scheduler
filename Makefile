CC = gcc
CFLAGS = -Wall -g
INCLUDE = -Iinclude
SRC = main.c scheduler.c
OUT = scheduler

all:
	$(CC) $(CFLAGS) $(SRC) $(INCLUDE) -o $(OUT)

clean:
	rm -f $(OUT)
