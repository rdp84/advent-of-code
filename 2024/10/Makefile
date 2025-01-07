CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
# CFLAGS = -Wall
OBJECTS	= day_10.o
OUTPUT	= -o ../../bin/day_10

day10: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_10.o: day_10.h

.PHONY: clean
clean:
	rm ../../bin/day_10 $(OBJECTS)
