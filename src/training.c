#include "../include/ids.h"

void run_training_mode(char *pathname, char** new_argv){
  // Attempt to load in previously made profile into a struct
  char* programName = getProgramName(pathname); // This will be the programName to look for when searching for a profile

  Profile *programProfile = (Profile*)loadProfile(programName);

  // Using the syscalls from the program, generate ngrams 
  ngram* ngrams = generateNgrams(pathname, new_argv);

  printNgrams(ngrams);

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

int main(){
	char * args[] = {"./testOpen", NULL};

	run_training_mode("/home/sekar/Desktop/360/testOpen", args);

	return 1;
}


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
