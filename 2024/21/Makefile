CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_21.o
OUTPUT	= -o ../../bin/day_21

day21: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_21.o: day_21.h

.PHONY: clean
clean:
	rm ../../bin/day_21 $(OBJECTS)
