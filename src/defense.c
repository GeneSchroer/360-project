#include "ids.h"

void writeErrorLog(ngram *list, char *programName, int ngramSize, int n);

int run_defense_mode(char *pathname, char** new_argv, int ngramSize){  
  pid_t child;
  char* programName = getProgramName(pathname); // program name
  Profile *profile = (Profile*)loadProfile(programName,ngramSize); // program's profile
  int i;//, j, k;
  //  int fd = open("outputfile", O_);
  ngram trav; // The current Ngram we are monitoring.
  int intrusion=0;

  ngram *list = malloc(sizeof(ngram));
  list[0].sysCalls = malloc(sizeof(int) * ngramSize);
  int n=0;
  
  // Load the up the Ngram with -1s, as we haven't stored syscalls yet
  trav.sysCalls = malloc(sizeof(int) * ngramSize);
  for(i = 0; i < ngramSize;++i){
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
      //  printf("%d\n", j++);
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
	  for(i=0;i<ngramSize;){
	    if(trav.sysCalls[i] == -1){
	      trav.sysCalls[i] = (int)regs.orig_eax;
	      ++i;
	      break;
	    }
	    ++i;
	  }
	  if(i == ngramSize)
	    haveNgram = 1;
	}
	else{
	  for(i=0;i<ngramSize-1;++i)
	    trav.sysCalls[i] = trav.sysCalls[i+1];
	  trav.sysCalls[i] = (int)regs.orig_eax;
	}

	// If the Ngram is not part of the profile,
	// note the invalid syscall and kill the program.
	if(haveNgram){
	  if(isValidNgram(trav, *profile) == 0){
	    printf("Invalid Ngram discovered: ");
	    for(i=0;i<ngramSize;++i)
	      printf("%d ", trav.sysCalls[i]);
	    printf("\n");
	    ptrace(PTRACE_KILL, child, NULL, NULL);
	    memcpy(list[n].sysCalls, trav.sysCalls, sizeof(int)*ngramSize);
	    ++n;
	    writeErrorLog(list, programName, ngramSize, n);


	    intrusion = 1;
	  }
	  // add the syscall to the list
	  else{
	    memcpy(list[n].sysCalls, trav.sysCalls, sizeof(int)*ngramSize);
	    ++n;
	    list = realloc(list, sizeof(ngram) * n + 1);
	    list[n].sysCalls = malloc(sizeof(int) * ngramSize);
	    
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
  else{
    printf("There was an inrustion\n");
  }

  return 0;
}


// Append the list of ngrams to the errorLog,
// or create one if it doesn't exist.
void writeErrorLog(ngram *list, char *programName, int ngramSize, int n){
  int i, j;
  char *errorLog = malloc(sizeof(char) * 256);
  strcpy(errorLog, "errorLog/");
  strcat(errorLog, programName);
  char sizeNgram[5];
  sprintf(sizeNgram, "%d", ngramSize);
  strcat(errorLog, sizeNgram);
  strcat(errorLog,"_errorLog.txt");
  char buf[5];
  int fd = open(errorLog, O_WRONLY | O_APPEND | O_CREAT,
	   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  
  //  printf("%d %d %d\n", ngramSize, n, fd);
  
  write(fd, "\n", sizeof(char));
  printf("%d %d %d\n", list[0].sysCalls[0], list[0].sysCalls[1], list[0].sysCalls[2]);
  for(i = 0; i<n;++i){
    for(j = 0; j<ngramSize;++j){
      sprintf(buf, "%d", list[i].sysCalls[j]);
      write(fd, buf, sizeof(char) * strlen(buf));
      write(fd, " ", sizeof(char));
    }
    write(fd, "\n", sizeof(char));
    
  }
  
  close(fd);
  
}
