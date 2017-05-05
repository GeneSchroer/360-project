#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
	FILE * fp;
	fp = fopen("sampletext", "r");
	char input[1000];
	fgets(input, 999, stdin);
	fclose(fp);
}
