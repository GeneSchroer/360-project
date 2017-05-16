#include "../include/ids.h"

void run_training_mode(char *pathname, char** new_argv){
  // Attempt to load in previously made profile into a struct
  char* programName = getProgramName(pathname); // This will be the programName to look for when searching for a profile

  Profile *programProfile = (Profile*)loadProfile(programName);

  programProfile->numCalled++;

  // Using the syscalls from the program, generate ngrams 
  ngram* ngrams = generateNgrams(pathname, new_argv);

  // Insert ngrams
  insertNgrams(programProfile, ngrams);

  //printNgrams(ngrams);

  writeProfile(programProfile, programName);

  // TODO fix this
  //freeProfile(*programProfile);
  
  return;
}

void printNgrams(ngram* ngrams){
	int i = 0;
	ngram pointer;

	while(1){
		pointer = ngrams[i];

		if(pointer.sysCalls == NULL){
			return;
		}

		printf("%d %d %d\n", pointer.sysCalls[0], pointer.sysCalls[1], pointer.sysCalls[2]);
		
		i++;
	}
}


/*int main(){
	//char * args[] = {"./testOpen", NULL};
	char * args[] = {NULL};

	run_training_mode("/home/sekar/Desktop/360/testOpen", args);

	return 1;
}
*/

/*
pid_t child;
  long orig_eax;
  child = fork();
  if(child==0){
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execv(pathname, new_argv);
  }
  else{
    wait(NULL);
    orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
    printf("The child made a system call %ld\n", orig_eax);
    ptrace(PTRACE_CONT, child, NULL, NULL);

  }
*/



/*
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
  //  child = fork();
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
      // if the ngram is not part of the profile, kill the program 
      if(isValidNgram(n, *profile) == 0){
	ptrace(PTRACE_KILL, child, NULL, NULL);
      }
      
    }
  }
  return 0;
*/
