CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_20.o
OUTPUT	= -o  ../../bin/day_20

day20: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_20.o: day_20.h

.PHONY: clean
clean:
	rm ../../bin/day_20 $(OBJECTS)
