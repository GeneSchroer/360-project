#ifndef IDSHEADER

#define IDSHEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>

#define NGRAM_SIZE 3
#define NUM_NGRAM_BUCKETS 4

// Holds all information for a program's IDS profile
typedef struct{
	// A lis of all directories and files accessible by this program
	char** directories;
	int numDirectories;
	
	// Number of times this program was called in training mode
	int numCalled;
	
	// List of 4 ngram buckets in which ngrams will be placed. The bucket number is determined by taking ngram->sysCalls[0] % 4
	// This simple hash will reduce search times for an existing ngram sequence in both training mode and defense mode
	ngramBucket* ngramBuckets;
	int numNgramBuckets;
}Profile;

// Holds a list of ngrams. Bucket number is determined using the algorithm mentioned above
typedef struct{
	int numNgrams;
	ngram* ngrams;
}ngramBucket;

// A list of at most three system calls recorded from the program
typedef struct{
  int* sysCalls;
}ngram;

#endif
