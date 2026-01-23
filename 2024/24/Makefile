CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
OBJECTS	= day_24.o
OUTPUT	= -o ../../bin/day_24

day24.o: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_24.o: day_24.h

.PHONY: clean
clean:
	rm ../../bin/day_24 $(OBJECTS)
