CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -O0 -pedantic -g
OBJECTS	= day_8.o hash_table.o
LINK    = -lm
OUTPUT	= -o ../../bin/day_8

day8: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(LINK) $(OBJECTS)

day_8.o: day_8.h
hash_table.o: day_8.h

.PHONY: clean
clean:
	rm ../../bin/day_8 $(OBJECTS)
