CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -pedantic -g -O0
OBJECTS	= day_3.o
OUTPUT	= -o ../../bin/day_3

day3: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_3.o: day_3.h

.PHONY: clean
clean:
	rm ../../bin/day_3 $(OBJECTS)
