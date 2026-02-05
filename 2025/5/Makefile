CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g -pedantic
# CFLAGS	= -Wall -g -pedantic -O0
OBJECTS	= day_5.o
OUTPUT	= -o ../../bin/day_5

day5: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_5.o: day_5.h

.PHONY: clean
clean:
	rm ../../bin/day_5 $(OBJECTS)
