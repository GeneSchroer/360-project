#include "../include/ids.h"
//#include "util.c"
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
  // Step 1: Load a profile
  char* programName = getProgramName(pathname);
  profile = (Profile*)loadProfile(programName);

  
  n.sysCalls = malloc(sizeof(int) * (NGRAM_SIZE + 1));



  // Step ? - Create a child process and execute the input program 
  child = fork();
  if(child==0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(pathname, new_argv);
  }
  else{
   	struct user_regs_struct regs;
	int status = 0;
	int check = 0;
	while(waitpid(child, &status, 0) && (!WIFEXITED(status))) {
		ptrace(PTRACE_GETREGS, child, NULL, &regs);

		//PTRACE_SYSCALL stops at beginning and end of syscall. should only log once

		//this marks beginning of syscall, should add to list
		if (regs.eax == -38) {
			check = 1;
		}

		//if check, then new syscall was added. generate the new ngrams
		if (check) {

		}

		//then check if these ngrams are allowed. if so, kill it with kill(pid, SIGKILL)

		//reset check for next iteration of loop
		check = 0;

		ptrace(PTRACE_SYSCALL, child, 0, 0);
	}
 }

  return 0;
}
