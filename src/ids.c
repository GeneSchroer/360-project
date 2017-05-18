#include "../include/ids.h"

extern char *optarg;

int main(int argc, char* argv[]){

	struct stat *buf = malloc(sizeof(struct stat));
	struct stat *lbuf = malloc(sizeof(struct stat));
	int opt;
	int nSize;


	 void run_defense_mode(char* pathname, char** new_argv, int ngramSize);
	 void run_training_mode(char* pathname, char** new_argv, int ngramSize);
	 
	if(argc<4){
	    printf("Usage: ./ids -T|-D NGRAMSIZE FILE [args] \n");
	    return -1;
	}

	char* pathname = argv[3];
	 
	if( stat(pathname, buf) == -1){
	printf("Error: Not a valid file!\n");
	return -1;
	}

	 
	 char **new_argv = malloc(sizeof(char**) * (argc-1) );
	 new_argv[0] = pathname;
	 int i, j;
	 for(i = 1, j = 3; j<argc; i++, j++){
	   new_argv[i] = argv[j];

	 }
	 new_argv[i]=NULL;


	 opt = argv[1][1];
	 nSize = atoi(argv[2]);
	switch(opt){
	 case 'D':
	   printf("Running Defense Mode\n");
	   run_defense_mode(pathname, new_argv, nSize);
	   return 0;
	    
	 case 'T':
	   printf("Running Training Mode\n");
	   run_training_mode(pathname, new_argv, nSize);
	   return 0;
	 default:
	   printf("Error: Not a valid option!\n");
	   return -1;
	 }
	  



	lstat(argv[3], lbuf);

}
