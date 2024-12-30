CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic
# CFLAGS = -Wall
OBJECTS	= day_7.o
OUTPUT	= -o ../../bin/day_7

day7: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_7.o:

.PHONY: clean
clean:
	rm ../../bin/day_7 $(OBJECTS)
