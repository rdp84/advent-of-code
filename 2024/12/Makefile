CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_12.o
OUTPUT	= -o ../../bin/day_12


day12: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_12.o: day_12.h

.PHONY: clean
clean:
	rm ../../bin/day_12 $(OBJECTS)
