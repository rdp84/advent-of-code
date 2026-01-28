CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -pedantic -g -O0
OBJECTS	= day_2.o
OUTPUT	= -o ../../bin/day_2

day2: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_2.o: day_2.h

.PHONY: clean
clean:
	rm ../../bin/day_2 $(OBJECTS)
