CC = g++
CFLAGS = -Wall -Wextra -std=c++11

EXEC = main

all: $(EXEC)

$(EXEC): water.cpp main.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(EXEC)