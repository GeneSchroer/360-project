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
	
}