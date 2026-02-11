CC	= gcc
CFLAGS	= -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Wall -O0 -g
OBJECTS	= day_7.o
OUTPUT	= -o ../../bin/day_7

day7: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(OBJECTS)

day_7.o: day_7.h

.PHONY: clean
clean:
	rm ../../bin/day_7 $(OBJECTS)
