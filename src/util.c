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

	FILE profile = fopen(profileName);

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

	// Read in all directories from profile file
	while(strstr(buf, "SYSCALLS") == NULL){
		char* dirBuf = malloc(strlen(buf));
		strcpy(buf,dirbuf);
		newProfile.directories[numDirectories] = &dirBuf;
		newProfile.numDirectories++;
		fgets(buf,256,profile);
	}

	// After all directories are loaded in, load in the syscalls
	fgets(buf,256);

	while(strstr(buf, "REGISTERS") == NULL){

	}

	return &newProfile;
}