#include "ids.h"
#include "tracer.h"

/*extern*/ long * syscalls;
/*extern*/ int syscallsLength;

char* getProgramName(char* path){
	char* programName = basename(path);
	return programName;
}

/*
* Gets syscalls from specifed file and generates all ngrams from it
*/
void* generateNgrams(const char *path, char *const argv[], int ngramSize){
	// First get the syscalls for the target program
    getSyscalls(path, argv);

	// List of ngrams to be generated from the list of syscalls
	ngram* ngrams;

	// The pointer of the list of three or fewer integers coresponding to the syscalls in the current ngram
	int* currentNgramPointer;

	// Allocate the space for the list of ngrams
	if(syscallsLength == 0){
		return NULL;
	}
	else if(syscallsLength < ngramSize){
		// Allocate space for one ngram
		ngrams = (ngram*)malloc(sizeof(ngram) * 2);

		// Allocate space for one int array the size of the number of syscalls
		currentNgramPointer = (int*)malloc(syscallsLength * sizeof(int));

		// Fill up that array
		for(int i = 0; i < syscallsLength; i++){
			currentNgramPointer[i] = (int)syscalls[i];
		}

		// Insert it into the list of ngrams
		ngrams[0].sysCalls = currentNgramPointer;
		ngrams[1].sysCalls = NULL;
		return ngrams;
	}
	else{
		// Allocate enough space for a list of ngrams that is the size of the syscalls array minus 2
		ngrams = (ngram*)malloc((syscallsLength-1) * sizeof(ngram));
		int counter = 0;
		for(int i = 0; i < syscallsLength - 2; i++){
			currentNgramPointer = (int*)malloc(ngramSize * sizeof(int));

			for(int j = 0; j < ngramSize; j++){
				currentNgramPointer[j] = syscalls[i + j];
			}

			ngrams[i].sysCalls = currentNgramPointer;
			counter++;
		}
		ngrams[counter].sysCalls = NULL;
		return ngrams;
	}
}

/*
* Takes a string buffer as input and returns the numerical values of the ngram
*/
void* getNgram(char* buf, int ngramSize){
	int n = 0;
	ngram* currentNgram = (ngram*)malloc(sizeof(ngram));
	int* sysCalls = malloc(ngramSize * sizeof(int));
	
	for(int i = 0; i < ngramSize; i++){
		sysCalls[i]=-1;
	}

	char* pch = strtok (buf," ");
	while (pch != NULL)
	{
    	sysCalls[n++] = atoi(pch);
    	pch = strtok (NULL, " ");
	}

	currentNgram->sysCalls = sysCalls;

	return currentNgram;
}

/*
* Insert an n gram into the bucket specified by the hash of the first element in the ngram
*/
void insertNgram(Profile *prof, ngram n){
	// Determine the bucket number by taking the first element in the ngram and moduloing it by 4
	int bucketNum = n.sysCalls[0] % NUM_NGRAM_BUCKETS;

	// Hold the current bucket to add to
	ngramBucket* currentBucket = &(prof->ngramBuckets[bucketNum]);

	currentBucket->numNgrams++;

	// Resize the current bucket to include room for the new ngram
	currentBucket->ngrams = (ngram*)realloc(currentBucket->ngrams, (currentBucket->numNgrams * sizeof(ngram)));

	// Add the new ngram to the bucket
	currentBucket->ngrams[currentBucket->numNgrams - 1] = n;
}

/*
* Attempts to load in a profile of a given program
* If there is no profile for the given program, then NULL is returned. Otherwise, a profile struct is
* created and returned to the caller
*/
void* loadProfile(char* programName, int ngramSize){
	// Get the name of the profile file given the program name
	char* profileName = (char*)malloc(256); 
	strcpy(profileName, "profiles/"); 
	strcat(profileName, programName);

	char sizeNgram[5];
	sprintf(sizeNgram, "%d", ngramSize);
	strcat(profileName,sizeNgram);

	strcat(profileName, "_profile.txt");

	// Try to open the file containing the program's profile
	FILE *profile = fopen(profileName, "r");

	// Free the memory associated with the profile name
	free(profileName);

	// Create a new Profile Struct and initialize it
	Profile *newProfile = (Profile*)malloc(sizeof(Profile));
	newProfile->numCalled = 0;

	newProfile->numNgramBuckets = NUM_NGRAM_BUCKETS;
	newProfile->ngramSize = ngramSize;

	// Initialize all of the ngramBuckets to a size of zero
	for(int i = 0; i < NUM_NGRAM_BUCKETS; i++){
		newProfile->ngramBuckets[i].numNgrams = 0;
		//newProfile->ngramBuckets[i].ngrams = (ngram*)malloc(sizeof(int) * ngramSize);
		newProfile->ngramBuckets[i].ngrams = (ngram*)malloc(sizeof(ngram));
	}
	// If there is no profile made for the current program then return the empty profile struct, otherwise fill it with the information from the file
	if(profile == NULL){
		return newProfile;
	}

	// The buf for reading from the profile file
	char buf[256];

	// Read a line from the profile file
	fgets(buf,256,profile);

	// Get the number of times the program was called
	int numCalled = atoi(buf);
	newProfile->numCalled = numCalled;
	// Get the ngram at the current line and add it to the profile
	while(fgets(buf,256,profile)){
		ngram* currentNgram = (ngram*)getNgram(buf, ngramSize);
		insertNgram(newProfile, *currentNgram);
	}

	// Close the file
	fclose(profile);

	return newProfile;
	
}

/*
* Writes a program's profile to memory
*/
void writeProfile(Profile* profile, char* programName){
	// Either open up the existing profile or create a new file given the specified program name
	char* profileName = (char*)malloc(256); 
	strcpy(profileName, "profiles/"); 
	strcat(profileName, programName);

	char sizeNgram[5];
	sprintf(sizeNgram, "%d", profile->ngramSize);
	strcat(profileName,sizeNgram);

	strcat(profileName, "_profile.txt");

	FILE *profileFile = fopen(profileName, "w");
printf("%s\n", profileName);
	// Write the number of times the program was called to the file on the first line
	fprintf(profileFile, "%d\n", profile->numCalled);

	/*
	// Loop through all of the directories and write each to a new line in the file
	for(int i = 0; i < profile->numDirectories; i++){
		fprintf(profileFile, "%s\n", profile->directories[i]);
	}
	*/
	
	// Loop through all of the ngrams and write each sequence on its own line in the file
	for(int i = 0; i < profile->numNgramBuckets; i++){
		for(int j = 0; j < profile->ngramBuckets[i].numNgrams; j++){
			int* currentNgram = profile->ngramBuckets[i].ngrams[j].sysCalls;
			for(int k = 0; k < profile->ngramSize; k++){
				fprintf(profileFile, "%d ", currentNgram[k]);
			}
			fprintf(profileFile, "\n");
		}
	}
	// Close the file
	fclose(profileFile);

	// Free the memory associated with profileName
	free(profileName);
}

/*
* Frees a profile's memory
*/
void freeProfile(Profile prof){
	// Free all ngrams and buckets
	for(int i = 0; i < prof.numNgramBuckets; i++){
		for(int j = 0; j < prof.ngramBuckets[i].numNgrams; j++){
			free(prof.ngramBuckets[i].ngrams[j].sysCalls);
		}
		free(&prof.ngramBuckets[i]);
	}
}

// Inserts a list of ngrams into a profile if they don't already exist in the profile
void insertNgrams(Profile *profile, ngram* ngrams){
	ngram pointer;
	int i = 0;
	int bucketNum = 0;
	
	while(1){
		pointer = ngrams[i];

		if(pointer.sysCalls == NULL){
			return;
		}
		
		bucketNum = pointer.sysCalls[0] % NUM_NGRAM_BUCKETS;
		if(!inBucket(pointer, profile->ngramBuckets[bucketNum], profile->ngramSize)){
			profile->ngramBuckets[bucketNum].numNgrams++;
			profile->ngramBuckets[bucketNum].ngrams = (ngram*)realloc(profile->ngramBuckets[bucketNum].ngrams, profile->ngramBuckets[bucketNum].numNgrams * sizeof(ngram));
			profile->ngramBuckets[bucketNum].ngrams[profile->ngramBuckets[bucketNum].numNgrams - 1] = pointer;
		}
		i++;
	}
}

/* Compares two ngrams to determine if they are the same */
/* returns 0 (false) if they are not, and 1 (true) otherwise */
int compareNgrams(ngram current, ngram trav, int ngramSize){
  int i;
  for(i = 0; i< ngramSize; ++i)
    if (current.sysCalls[i] != trav.sysCalls[i])
      return 0;
  
  return 1;
}

/* iterates through an ingram bucket to determine */
/* if the bucket contains an ngram */
/* returns 0 (false) if it does not and 1 (true) if it does */
int inBucket(ngram current, ngramBucket bucket, int ngramSize){
  int i;
  for(i=0; i < bucket.numNgrams; ++i)
    if( compareNgrams(current, bucket.ngrams[i], ngramSize) == 1)
      return 1;
  return 0;
}

int isValidNgram(ngram current, Profile profile){
  int i;
  for(i=0; i< profile.numNgramBuckets; ++i)
    if( inBucket(current, profile.ngramBuckets[i], profile.ngramSize) == 1)
      return 1;
  return 0;
}

