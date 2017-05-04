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

void printCalls(int * array, int max);

int findNull(char * start, int length);

void * getFileName(pid_t child, long address);
