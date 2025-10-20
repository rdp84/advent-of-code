CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_14.o
OUTPUT  = -o ../../bin/day_14

day14: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_14.o: day_14.h

.PHONY: clean
clean:
	rm ../../bin/day_14 $(OBJECTS)
