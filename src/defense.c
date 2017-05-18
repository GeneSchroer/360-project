#include "ids.h"


int run_defense_mode(char *pathname, char** new_argv, int ngramSize){  
  pid_t child;
  char* programName = getProgramName(pathname); // program name
  Profile *profile = (Profile*)loadProfile(programName,ngramSize); // program's profile
  int i, j, k;
  //  int fd = open("outputfile", O_);
  ngram trav; // The current Ngram we are monitoring.
  int intrusion=0;

  int unfilled=1; // flag to determine if the traversal ngram is filled or not
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
	  for(i=0;i<NGRAM_SIZE;){
	    if(trav.sysCalls[i] == -1){
	      trav.sysCalls[i] = (int)regs.orig_eax;
	      ++i;
	      break;
	    }
	    ++i;
	  }
	  if(i == NGRAM_SIZE)
	    haveNgram = 1;
	}
	else{
	  for(i=0;i<NGRAM_SIZE-1;++i)
	    trav.sysCalls[i] = trav.sysCalls[i+1];
	  trav.sysCalls[i] = (int)regs.orig_eax;
	}

	// If the Ngram is not part of the profile,
	// note the invalid syscall and kill the program.
	if(haveNgram){
	  if(isValidNgram(trav, *profile) == 0){
	    printf("Invalid Ngram discovered: ");
	    for(i=0;i<NGRAM_SIZE;++i)
	      printf("%d ", trav.sysCalls[i]);
	    printf("\n");
	    ptrace(PTRACE_KILL, child, NULL, NULL);
	    intrusion = 1;
	  }
	  else{
	    //	    printf("Good ngram: %d %d %d\n", trav.sysCalls[0],
	    //	   trav.sysCalls[1], trav.sysCalls[2]);
	  }
	
	}
    

      }	       

    //reset check for next iteration of loop
    sysCheck = 0;
    
    ptrace(PTRACE_SYSCALL, child, 0, 0);
    
    }
  }

  if(!intrusion)
    printf("IDS did not detect any intrusions\n");

  return 0;
}
