#include "ids.h"
#include "tracer.h"

extern long * syscalls;
extern int syscallsLength;

// Generates 
void generateNgrams(){

}

void run_training_mode(char *pathname, char** new_argv){
  // Attempt to load in previously made profile into a struct
  char* programName; // This will be the programName to look for when searching for a profile
  Profile programProfile = *loadProfile(programName);

  // Get syscalls from the target program
  getSyscalls();

  // Using the syscalls from the program, generate ngrams 
  generateNgrams();

  writeProfile(programProfile, programName);

  freeProfile(programProfile);
  return;
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
