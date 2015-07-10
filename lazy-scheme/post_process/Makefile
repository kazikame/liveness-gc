CFLAGS=-Wall -W -O -O4
all: process process_fast

process: process.c
	gcc $(CFLAGS) -o process process.c

process_fast: process_fast.cpp
	g++ $(CFLAGS) -o process_fast process_fast.cpp

.PHONY: clean

clean:
	$(RM) -f process process_fast
