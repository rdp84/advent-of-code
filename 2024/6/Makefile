CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -pedantic 
# CFLAGS	= -Wall
OBJECTS	= day_6.o
OUTPUT	= -o ../../bin/day_6

day6: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_6.o: day_6.h

.PHONY: clean
clean:
	rm ../../bin/day_6 $(OBJECTS)

