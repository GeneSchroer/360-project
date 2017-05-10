#include "tracer.h"

extern long * syscalls;
extern int syscallsLength;

int main() {
	getSyscalls();
	int counter = 0;
	for(; counter < syscallsLength; counter++) {
		printf("syscall #: %ld\n", syscalls[counter]);
	}
	freeAll();
	return 1;
}
