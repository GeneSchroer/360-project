#include "trackCF.h"

//print contents of frequency table
void printCalls(int * array, int max) {
	int counter = 0;
	printf("\nSyscall\t|\tFrequency\n");
	for(; counter < max; counter++) {
		if (array[counter] > 0)
			printf("%d\t:\t%d times\n", counter, array[counter]);
	}
}

//checks for occurrence of null character within length of start
int findNull(char * start, int length) {
	int counter = 0;
	for(; counter < length; counter++) {
		if (start[counter] == '\0')
			return 1;
	}
	return 0;
}

//given the start address and the pid of the child process, get the string at that address
//the pointer returned by this must be freed 
void * getFileName(pid_t child, long address) {
	int foundNull = 0;
	int initSize = 32;
	void * ret = malloc(initSize);
	long counter = 0;
	int checkedSize = 0;
	while(!foundNull) {
		//get the data. ptrace only returns words, so need to grab data a word at a time
		long data = ptrace(PTRACE_PEEKDATA, child, address + sizeof(long) * counter, NULL);
		//copy the data to the allocated section
		memcpy(ret + sizeof(long) * counter, &data, sizeof(long));
		//check if the end of the string (a null byte) has been found
		if (!findNull(ret, sizeof(long) * (counter+1))) {
			//if not then update var's and continue
			counter++;
			checkedSize += 4;
		}
		else
			break;
		//is the string buffer maxed out?
		if (checkedSize == initSize) {
			initSize *= 2;
			ret = realloc(ret, initSize);
		}
	}
	return ret;
}

int main() {
	pid_t child;	
	//keep track of all syscalls
	int countCalls[338] = {0};
	child = fork();
	if (child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);		

//		execl("/bin/ls", "ls", NULL);
//		execl("/usr/bin/cal", "cal", NULL);
//		execl("/usr/bin/ncal", "ncal", NULL);
//		execl("/bin/date", "date", NULL);
//		execl("/bin/pwd", "pwd", NULL);
//		execl("/usr/bin/time", "time", NULL);
//		execl("/usr/bin/head", "head", "sampletext", NULL);
//		execl("/bin/cat", "cat", "sampletext", NULL);
//		execl("/bin/chmod", "chmod", "+r", "note", NULL);
		execl("/home/sekar/Desktop/360/testOpen", "./testOpen", NULL);
	}
	else {
		struct user_regs_struct regs;
		int status = 0;
		while(waitpid(child, &status, 0) && (!WIFEXITED(status))) {
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			printf("syscall %ld\n", regs.orig_eax);
			countCalls[regs.orig_eax]++;
			
			//the open or create syscall
			if ((regs.orig_eax == 5) || (regs.orig_eax == 8)) {
				void * fileName = getFileName(child, regs.ebx);
				printf("file: %s\n", (char *) fileName);
				free(fileName);
			}
			//the openat syscall
			if (regs.orig_eax == 295) {
				void * fileName = getFileName(child, regs.ecx);
				printf("file: %s\n", (char *) fileName);
				free(fileName);
			}
			//the read, write, or close syscall
			if ((regs.orig_eax == 3) || (regs.orig_eax == 4) || (regs.orig_eax == 6)) {
				int actualFd = ptrace(PTRACE_PEEKDATA, child, regs.ebx, NULL);
				printf("descriptor:%d\n", actualFd);
			}

			ptrace(PTRACE_SYSCALL, child, 0, 0);
		}

	}	
	printCalls(countCalls, 338);
	return 0;
}
