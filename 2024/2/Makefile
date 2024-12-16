CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
# CFLAGS	= -Wall
OBJECTS	= day_2.o
OUTPUT	= -o ../../bin/day_2

day2: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(OUTPUT)

day_2.o:

.PHONY: clean
clean:
	rm ../../bin/day_2 $(OBJECTS)
