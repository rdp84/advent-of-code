CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
# CFLAGS = -Wall
OBJECTS	= day_4.o
OUTPUT	= -o ../../bin/day_4

day4: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_4.o: day_4.h

.PHONY: clean
clean:
	rm ../../bin/day_4 $(OBJECTS)
