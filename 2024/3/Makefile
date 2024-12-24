CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
# CFLAGS	= -Wall
OBJECTS	= day_3.o
OUTPUT	= -o ../../bin/day_3

day3: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_3.o:

.PHONY: clean
clean:
	rm ../../bin/day_3 $(OBJECTS)
