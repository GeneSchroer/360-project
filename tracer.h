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
	long syscall;
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

int syscallLength();

int printCallNum();

void printRegisterRecords(FILE * output);

void printFileRecords(FILE * output);

int addRegisterRecord(int h, long a, long b, long c, long d, long e, long f, long g);

int addFileRecord(long a, char * b, char * c);

void freeAll();

void printCalls(int * array, int max);

int findNull(char * start, int length);

void * getString(pid_t child, long address);

int getLength(int number);

int append(char * s, int size, char c);

void * getFileName(unsigned int fd, pid_t child);

void exitGracefully();

void getSyscalls(const char *path, char *const argv[]);

