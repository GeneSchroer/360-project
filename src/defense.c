#include "ids.h"

int run_defense_mode(char *pathname, char** new_argv){
  pid_t child;
  long orig_eax, eax, ebx, edx;
  int status;
  int insyscall = 0;
  ngram n;
  n->syscalls = malloc(sizeof(int) * (NGRAM_SIZE + 1));
  child = fork();
  if(child==0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(pathname, new_argv);
  }
  else{
    while(1){
      wait(&status);
      if(WIFEXITED(status))
	break;
	orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
	//	if(orig_eax == SYS_write){
	  //  if(insyscall == 0){
	  //insyscall = 1;
	    ebx = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
	    edx = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

	    //	ebx = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
	    printf("Write called with: %ld %ld %ld\n", orig_eax, ebx, edx);
	    //	  }
	//else{
	//  eax = ptrace(PTRACE_PEEKUSER, child, 4 * EAX, NULL);
	//  printf("Write returned "
	//		   "with %ld\n", eax);
	    //insyscall = 0;
    //}
	//}
	ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
  }
  return 0;
}
