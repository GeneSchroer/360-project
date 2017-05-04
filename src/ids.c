#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

extern char *optarg;

int main(int argc, char* argv[]){

struct stat *buf = malloc(sizeof(struct stat));
struct stat *lbuf = malloc(sizeof(struct stat));
int opt;

if(argc<3){

    printf("Usage: ./ids -T|-D FILE [args] \n");
    return -1;
}

if( stat(argv[2], buf) == -1){
printf("Error: Not a valid file!\n");
return -1;
}

char **new_argv = (argv+3);



if(opt = getopt(argc, argv[1], "DT") != -1){
switch(opt){
 case 'D':
   printf("Running defence mode\n");
   break;
    
 case 'T':
   printf("Running training mode\n");
      break;
 default:
   printf("Error: Not a valid option!\n");
   return -1;
    }
  }
  else{
    printf("Error: Not an option!\n");
    return -1;
  }



lstat(argv[2], lbuf);

}
