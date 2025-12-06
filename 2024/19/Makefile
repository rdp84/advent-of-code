CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
OBJECTS	= day_19.o
OUTPUT	= -o ../../bin/day_19

day19:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_19.o: day_19.h

.PHONY: clean
clean:
	rm ../../bin/day_19 $(OBJECTS)
