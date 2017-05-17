#include "ids.h"


int run_defense_mode(char *pathname, char** new_argv){  
  pid_t child;
  char* programName = getProgramName(pathname); // program name
  Profile *profile = (Profile*)loadProfile(programName); // program's profile
  int i, j, k;
  //  int fd = open("outputfile", O_);
  ngram trav; // The current Ngram we are monitoring.


  //for(i = 0; i < NUM_NGRAM_BUCKETS; ++i){
    //for(j = 0; ;)
  
  // Load the up the Ngram with -1s, as we haven't stored syscalls yet
  trav.sysCalls = malloc(sizeof(int) * (NGRAM_SIZE + 1));
  for(i = 0; i < NGRAM_SIZE;++i){
    trav.sysCalls[i] = -1;
  }

  
  // Step ? - Create a child process and execute the input program 
  child = fork();
  if(child==0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(pathname, new_argv);
  }
  else{
    struct user_regs_struct regs; // holds registers when tracing/
    int status = 0;
    int sysCheck = 0; // check if we're at the beginning of a syscall
    int haveNgram = 0; // flag to determine if there have been enough syscalls
                       // to form a complete ngram
    // sysCheck = 1;
    while(1){

      wait(&status);
      if(WIFEXITED(status)||WIFSIGNALED(status))
	break;
      ptrace(PTRACE_GETREGS, child, NULL, &regs);
      
      //PTRACE_SYSCALL stops at beginning and end of syscall. should only log once
      
      //this marks beginning of syscall, should add to list
      if (regs.eax == -38) {
		sysCheck = 1;
      }
      
      // if sysCheck is true, then new syscall was added.
      // generate the new ngrams
      if (sysCheck) {
	//if the program hasn't made enough sysCalls to create a complete ngram,
	//then fill up a Ngram variable with sysCall numbers
	if(!haveNgram){
	  if(trav.sysCalls[0] == -1)
	    trav.sysCalls[0] = (int)regs.orig_eax;
	  else if(trav.sysCalls[1] == -1)
	    trav.sysCalls[1] = (int)regs.orig_eax;
	  else{
	    trav.sysCalls[2] = (int)regs.orig_eax;
	    haveNgram = 1;
	  }
	}

	// If the Ngram is not part of the profile,
	// note the invalid syscall and kill the program.
	else if(isValidNgram(trav, *profile) == 0){
	  printf("Invalid Ngram discovered: "
		 "%d, %d, %d\n", trav.sysCalls[0],
		 trav.sysCalls[1], trav.sysCalls[2]);
	  ptrace(PTRACE_KILL, child, NULL, NULL);
	}
	else{
	  printf("Good ngram: %d %d %d\n", trav.sysCalls[0],
		 trav.sysCalls[1], trav.sysCalls[2]);
	  printf("Next ngram\n");
	  trav.sysCalls[2] = trav.sysCalls[1];
	  trav.sysCalls[1] = trav.sysCalls[0];
	  trav.sysCalls[0] = (int)regs.orig_eax;
	}
	
      }

	       

      //reset check for next iteration of loop
      sysCheck = 0;
		
      ptrace(PTRACE_SYSCALL, child, 0, 0);
    }
  }
  return 0;
}
