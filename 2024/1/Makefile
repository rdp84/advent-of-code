CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
# CFLAGS	= -Wall
OBJECTS	= day_1.o
OUTPUT	= -o ../../bin/day_1

day1: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(OUTPUT)

day_1.o:

.PHONY: clean
clean:
	rm ../../bin/day_1 $(OBJECTS)
