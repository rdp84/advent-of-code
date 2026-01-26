CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
OBJECTS	= day_1.o
OUTPUT	= -o ../../bin/day_1

day1: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_1.o: day_1.h

.PHONY: clean
clean:
	rm ../../bin/day_1 $(OBJECTS)
