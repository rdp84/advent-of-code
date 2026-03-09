CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -pedantic -g -O0
OBJECTS	= queue.o day_10.o
LINK	= -lm
OUTPUT	= -o ../../bin/day_10

day10: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(LINK) $(OBJECTS)

queue.o:  queue.h
day_10.o: day_10.h

.PHONY: clean
clean:
	rm ../../bin/day_10 $(OBJECTS)
