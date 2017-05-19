#ifndef IDSHEADER

#define IDSHEADER

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

//#define NGRAM_SIZE 3
#define HASH 4
#define NUM_NGRAM_BUCKETS 4

// A list of at most n system calls recorded from the program
typedef struct{
  int* sysCalls;
}ngram;

// Holds a list of ngrams. Bucket number is determined using the algorithm mentioned above
typedef struct{
	int numNgrams;
	ngram* ngrams;
}ngramBucket;

// Holds all information for a program's IDS profile
typedef struct{
	// The size of ngrams in this profile
	int ngramSize;
	
	// Number of times this program was called in training mode
	int numCalled;
	int numNgramBuckets;
	// List of 4 ngram buckets in which ngrams will be placed. The bucket number is determined by taking ngram->sysCalls[0] % 4
	// This simple hash will reduce search times for an existing ngram sequence in both training mode and defense mode
	ngramBucket ngramBuckets[NUM_NGRAM_BUCKETS];
}Profile;
//#endif
//void run_training_mode(char *pathname, char** new_argv);

void printNgrams(ngram* ngrams);

void* generateNgrams(const char *path, char *const argv[], int ngramSize);

void* getNgram(char* buf, int ngramSize);

void insertNgram(Profile *prof, ngram n);

void* loadProfile(char* programName, int ngramSize);

void writeProfile(Profile* profile, char* programName);

void freeProfile(Profile prof);

int compareNgrams(ngram current, ngram trav, int ngramSize);

int inBucket(ngram current, ngramBucket bucket, int ngramSize);

int isValidNgram(ngram current, Profile profile);

char* getProgramName(char* path);

void insertNgrams(Profile *profile, ngram* ngrams);




#endif
