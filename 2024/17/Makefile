CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_17.o
OUTPUT	= -o ../../bin/day_17

day17:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_17.o: day_17.h

.PHONY: clean
clean:
	rm ../../bin/day_17 $(OBJECTS)
