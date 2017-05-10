#include "ids.h"

/*
* Attempts to load in a profile of a given program
* If there is no profile for the given program, then NULL is returned. Otherwise, a profile struct is
* created and returned to the caller
*/
void* loadProfile(char* programName){
	// Get the name of the profile file given the program name
	char* profileName = strcat("../profiles/", programName);
	profileName = strcat(profileName, "_profile.txt");

	FILE profile = fopen(profileName, "r");

	// If there is no profile made for the current program then return NULL
	if(profile == NULL){
		return NULL;
	}

	// Otherwise create a new struct and fill it with all information from the file
	Profile newProfile;
	newProfile.numDirectories = 0;
	newProfile.directories = (char*)malloc(sizeof(char*));
	newProfile.sysCalls = (int*)malloc(sizeof(int*));
	newProfile.numSysCalls = 0;

	// The buf for reading from the profile file
	char* buf[256];

	// Read a line from the profile file
	fgets(buf,256,profile);

	// Get the number of times the program was called
	int numCalled = atoi(buf);
	newPorfile.numCalled = numCalled;

	fgets(buf,256,profile);

	// Read in all directories from profile file
	while(strstr(buf, "SYSCALLS") == NULL){
		// Allocate memory for the directory string
		char* dirBuf = malloc(strlen(buf));

		// Copy that string to memory
		strcpy(buf,dirbuf);

		// Resize the directories array to store that new string
		newProfile.directories = (char*)realloc(newProfile.directories, newProfile.numDirectories + 1 * sizeof(char*));
		
		// Add in the new directory
		newProfile.directories[newProfile.numDirectories++] = &dirBuf;
		
		// Fetch the next line of the file
		fgets(buf,256,profile);
	}

	// After all directories are loaded in, load in the syscalls
	fgets(buf,256);

	while(strstr(buf, "REGISTERS") == NULL){
		// Convert the buf to an int
		int syscall = atoi(buf);

		// Resize the array of ints to add the new syscall
		newProfile.sysCalls = (int*)realloc(newProfile.sysCalls, newProfile.numSysCalls + 1 * sizeof(int));

		// Insert the new syscall to the array
		newProfile.sysCalls[newProfile.numSysCalls++] = syscall;

		// Fetch the next line`
		fgets(buf,256,profile);
	}

	return &newProfile;
}

/*
*	Given a profile struct, writes all data to a file for later use by training runs or a defense run
*/
void writeProfile(Profile* profile, char* programName){
	// Either open up the existing profile or create a new file given the specified program name
	char* profileName = strcat("../profiles/", programName);
	profileName = strcat(profileName, "_profile.txt");
	FILE profileFile = fopen(profileName, "w");


	// Write the number of times the program was called to the file on the first line
	fprintf(profileFile, "%d\n", profile->numCalled);

	// Loop through all of the directories
	for(int i = 0; i < profile->numDirectories; i++){
		fprintf(profileFile, "%s\n", profile->directories[i]);
	}
	for(int i = 0; i < profile->numSysCalls; i++){
		fprintf(profileFile, "%d\n", profile->sysCalls[i]);
	}
}




int isValidNgram(ngram current, ngram trav){
  int i;
  for(i = 0; i< NGRAM_SIZE; ++i)
    if (current[i] != trav[i])
      return 0;
  
  return 1;
}
