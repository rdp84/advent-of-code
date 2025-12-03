CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_18.o
OUTPUT	= -o ../../bin/day_18

day18:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_18.o: day_18.h

.PHONY: clean
clean:
	rm ../../bin/day_17 $(OBJECTS)
