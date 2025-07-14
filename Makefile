CC = gcc
CFLAGS = -Wall -g
INCLUDE = -Iinclude
SRC = src/main.c src/scheduler.c src/thread.c
OUT = scheduler

all:
	$(CC) $(CFLAGS) $(SRC) $(INCLUDE) -o $(OUT)

clean:
	rm -f $(OUT)
