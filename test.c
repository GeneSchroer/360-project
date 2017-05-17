#include "tracer.h"

extern long * syscalls;
extern int syscallsLength;

int main() {
	char * args[] = {"./testOpen", NULL};
	getSyscalls("/home/sekar/Desktop/360/testOpen", args);
	int counter = 0;
	//this prints out all the syscalls in chronological order
	for(; counter < syscallsLength; counter++) {
		printf("syscall #: %ld\n", syscalls[counter]);
	}
	freeAll();
	return 1;
}
