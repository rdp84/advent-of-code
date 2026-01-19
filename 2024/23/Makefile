CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
OBJECTS	= day_23.o
OUTPUT	= -o ../../bin/day_23

day23: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_23.o: day_23.h

.PHONY: clean
clean:
	rm ../../bin/day_23 $(OBJECTS)
