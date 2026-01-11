CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_22.o
OUTPUT	= -o ../../bin/day_22

day22: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_22.o: day_22.h

.PHONY: clean
clean:
	rm ../../bin/day_22 $(OBJECTS)

