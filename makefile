CC=gcc

CFLAGS=-g -Wall -Werror

BINARIES = trackCF lengths testOpen test

OBJECTS = tracer.o


all: $(BINARIES)

test: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) test.c -o linkTest

train: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) src/training.c src/util.c -o trainTest 

trackCF: 
	$(CC) $(CFLAGS) -o trackCF trackCF.c

lengths:
	$(CC) $(CFLAGS) -o lengths lengths.c

testOpen:
	$(CC) $(CFLAGS) -o testOpen testOpen.c

ids:
	$(CC) include/ids.h include/tracer.h src/tracer.c src/ids.c src/training.c src/defense.c src/util.c -o IDS -Iinclude

clean:
	rm -f $(OBJECTS) $(BINARIES) 
