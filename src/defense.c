#include "ids.h"
#include "util.c"
//char* getProgramName(char* pathname);
//int isValidNgram(ngram current, Profile profile);
//void* loadProfile(char* pathname);

int run_defense_mode(char *pathname, char** new_argv){
  pid_t child;
  long orig_eax, eax, ebx, edx;
  int status;
  int insyscall = 0;
  ngram n;
  Profile *profile = malloc(sizeof(Profile));
  int i;
  /* Step 1: Load a profile */
  char* programName = getProgramName(pathname);
  profile = (Profile*)loadProfile(programName);

  
  n.sysCalls = malloc(sizeof(int) * (NGRAM_SIZE + 1));



  /* Step ? - Create a child process and execute the input program */
  child = fork();
  if(child==0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(pathname, new_argv);
  }
  else{
    while(1){
      for(i = 0; i<NGRAM_SIZE; ++i){
	
	wait(&status);
	if(WIFEXITED(status))
	  break;
	orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
	//	if(orig_eax == SYS_write){
	//  if(insyscall == 0){
	//insyscall = 1;
	n.sysCalls[i] = (int) orig_eax;
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
      /* if the ngram is not part of the profile, kill the program */
      if(isValidNgram(n, *profile) == 0){
	ptrace(PTRACE_KILL, child, NULL, NULL);
      }
      
    }
  }
  return 0;
}
