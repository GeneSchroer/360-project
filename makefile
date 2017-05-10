CC=gcc

CFLAGS=-g -Wall -Werror

BINARIES = trackCF lengths testOpen test

OBJECTS = tracer.o


all: $(BINARIES)

test: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) test.c -o linkTest 

trackCF: 
	$(CC) $(CFLAGS) -o trackCF trackCF.c

lengths:
	$(CC) $(CFLAGS) -o lengths lengths.c

testOpen:
	$(CC) $(CFLAGS) -o testOpen testOpen.c

ids:
	$(CC) include/ids.h src/ids.c src/training.c src/defense.c -o IDS -Iinclude

clean:
	rm -f $(OBJECTS) $(BINARIES) 
