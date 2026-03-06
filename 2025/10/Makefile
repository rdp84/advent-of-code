CC	= gcc
CFLAGS	= -Iz3/include -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -pedantic -g
# CFLAGS	= -Iz3/include -Wall -pedantic -g -O0
OBJECTS	= queue.o solver.o day_10.o
LIB	= z3/libz3.dylib
# LIB     = z3/libz3.so
LINK	= -lm
OUTPUT	= -o ../../bin/day_10

day10: $(OBJECTS)
	$(CC) $(CFLAGS) $(OUTPUT) $(LINK) $(OBJECTS) $(LIB)

queue.o:  queue.h
solver.o: solver.h
day_10.o: day_10.h

.PHONY: clean
clean:
	rm ../../bin/day_10 $(OBJECTS)
