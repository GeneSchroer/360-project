#include "../include/ids.h"

extern char *optarg;

int main(int argc, char* argv[]){

	struct stat *buf = malloc(sizeof(struct stat));
	struct stat *lbuf = malloc(sizeof(struct stat));
	int opt;


	 void run_defense_mode(char* pathname, char** new_argv);
	 void run_training_mode(char* pathname, char** new_argv);
	 
	if(argc<3){
	    printf("Usage: ./ids -T|-D FILE [args] \n");
	    return -1;
	}

	char* pathname = argv[2];
	 
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
	switch(opt){
	 case 'D':
	   printf("Running Defense Mode\n");
	   run_defense_mode(pathname, new_argv);
	   return 0;
	    
	 case 'T':
	   printf("Running Training Mode\n");
	   run_training_mode(pathname, new_argv);
	   return 0;
	 default:
	   printf("Error: Not a valid option!\n");
	   return -1;
	 }
	  



	lstat(argv[2], lbuf);

}
