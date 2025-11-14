CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_16.o
OUTPUT	= -o ../../bin/day_16

day16:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_16.o: day_16.h

.PHONY: clean
clean:
	rm ../../bin/day_15 $(OBJECTS)
