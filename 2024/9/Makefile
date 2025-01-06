CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
# CFLAGS	= -Wall
OBJECTS	= day_9.o
OUTPUT	= -o ../../bin/day_9

day9: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_9.o: day_9.h

.PHONY: clean
clean:
	rm ../../bin/day_9 $(OBJECTS)
