CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_13.o
OUTPUT	= -o ../../bin/day_13

day13: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_13.o: day_13.h

.PHONY: clean
clean:
	rm ../../bin/day_13 $(OBJECTS)
