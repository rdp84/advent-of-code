CC	= gcc
# CFLAGS	= -Wall
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
OBJECTS	= day_8.o
OUTPUT	= -o ../../bin/day_8

day8: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_8.o: day_8.h

.PHONY: clean
clean:
	rm ../../bin/day_8 $(OBJECTS)
