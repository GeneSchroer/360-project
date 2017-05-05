#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


void main() {
	FILE * fp;
	fp = fopen("sampletext", "r");
	int fd;
	fd = fileno(fp);
	struct stat sb;
	ino_t inodeNumber;
	if (fstat(fd, &sb) == -1) {
		printf("here");
	}
	else {
		printf("inode: %lu\n", sb.st_ino);
		printf("fd: %d\n", fd);
	}
	char abc[200];
	fgets(abc, 200, stdin);
	fclose(fp);	
}
