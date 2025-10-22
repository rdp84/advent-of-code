CC	= gcc
CFLAGS  = -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS = day_15.o
OUTPUT	= -o ../../bin/day_15

day15: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_15.o: day_15.h

.PHONY: clean
clean:
	rm ../../bin/day_15 $(OBJECTS)
