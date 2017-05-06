#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>
#include <sys/reg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>

struct files {
	struct files * next;
	int syscall;
	char * name1;
	char * name2;
};
typedef struct files files;

//start indicates start(1) or end(0) of syscall. 2 means special case
struct registers {
	struct registers * next;
	int start;
	long orig_eax;
	long eax;
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
};
typedef struct registers registers;

void printCalls(int * array, int max);

int findNull(char * start, int length);

void * getString(pid_t child, long address);
