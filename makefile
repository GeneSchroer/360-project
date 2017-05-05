CC=gcc

CFLAGS=-Wall -Werror

OBJECTS = trackCF lengths testOpen


all: $(OBJECTS)

trackCF: 
	$(CC) $(CFLAGS) -o trackCF trackCF.c

lengths:
	$(CC) $(CFLAGS) -o lengths lengths.c

testOpen:
	$(CC) $(CFLAGS) -o testOpen testOpen.c

ids:
	$(CC) include/ids.h src/ids.c src/training.c src/defense.c -o IDS -Iinclude

clean:
	rm $(OBJECTS) 
