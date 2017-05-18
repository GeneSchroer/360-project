#include "ids.h"

void run_training_mode(char *pathname, char** new_argv, int ngramSize){
  // Attempt to load in previously made profile into a struct
  char* programName = getProgramName(pathname); // This will be the programName to look for when searching for a profile

  Profile *programProfile = (Profile*)loadProfile(programName, ngramSize);

  programProfile->numCalled++;

  // Using the syscalls from the program, generate ngrams 
  ngram* ngrams = generateNgrams(pathname, new_argv, ngramSize);

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

