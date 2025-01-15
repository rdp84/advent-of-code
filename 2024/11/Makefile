CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
# CFLAGS = -Wall
OBJECTS	= day_11.o
OUTPUT	= -o ../../bin/day_11

day11: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_11.o: day_11.h

.PHONY: clean
clean:
	rm ../../bin/day_11 $(OBJECTS)

