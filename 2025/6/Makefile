CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -g -O0
OBJECTS	= day_6.o
OUTPUT	= -o ../../bin/day_6

day6: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_6.o: day_6.h

.PHONY: clear
clear:
	rm ../../bin/day_6 $(OBJECTS)
