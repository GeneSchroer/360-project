#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>
#include <sys/reg.h>
#include <stdlib.h>

void printCalls(int * array, int max) {
	int counter = 0;
	printf("\nSyscall\t|\tFrequency\n");
	for(; counter < max; counter++) {
		if (array[counter] > 0)
			printf("%d\t:\t%d times\n", counter, array[counter]);
	}
}

int main() {
	pid_t child;
//	long orig;
	//keep track of all syscalls
	int countCalls[338] = {0};
	child = fork();
	if (child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		
//		kill(getpid(), SIGSTOP);

//		FILE *fp;
//		int c;
//		fp = fopen("sampletext", "r");
//		if (fp) {
//			while ((c = getc(fp)) != EOF)
//				putchar(c);
//			fclose(fp);
//		}

//		execl("/bin/ls", "ls", NULL);
		execl("/usr/bin/cal", "cal", NULL);
//		execl("/usr/bin/ncal", "ncal", NULL);
//		execl("/bin/date", "date", NULL);
//		execl("/bin/pwd", "pwd", NULL);
//		execl("/usr/bin/time", "time", NULL);
//		execl("/usr/bin/head", "head", "sampletext", NULL);
//		execl("/bin/cat", "cat", "sampletext", NULL);
//		execl("/bin/chmod", "chmod", "+r", "note", NULL);
	}
	else {
		struct user_regs_struct regs;
		int status = 0;
		while(waitpid(child, &status, 0) && (!WIFEXITED(status))) {
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			printf("syscall %ld\n", regs.orig_eax);
			countCalls[regs.orig_eax]++;
			ptrace(PTRACE_SYSCALL, child, 0, 0);
		}


//		ptrace(PTRACE_GETREGS, child, NULL, &regs);
//		orig = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
//		printf("The child made a system call of %ld\n", orig);
//		printf("%lX\n", regs.orig_eax);
//		printf("%lX\n", regs.eax);
//		printf("%lX\n", regs.eip);
//		ptrace(PTRACE_CONT, child, NULL, NULL);
//		ptrace(PTRACE_DETACH, child, NULL, 0);
	}	
	printCalls(countCalls, 338);
	return 0;
}
