#ifndef IDSHEADER

#define IDSHEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>

typedef struct{
	char** directories;
	size_t numDirectories;
	char** sysCalls;
	size_t numSysCalls;
}profile;

#endif
