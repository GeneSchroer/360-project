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

clean:
	rm $(OBJECTS) 
