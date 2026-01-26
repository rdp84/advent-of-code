CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
OBJECTS	= day_25.o
OUTPUT	= -o ../../bin/day_25

day25: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_25.o: day_25.h

.PHONY: clean
clean:
	rm ../../bin/day_25 $(OBJECTS)
