#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	FILE *fp;
	fp = fopen("note", "r");
	if (fp) {
		fclose(fp);
	}


	FILE *fp2;
	fp2 = fopen("sample", "ab+");
	int counter = 10000;

	FILE *fp3;
	fp3 = fopen("README.md", "r");

	while (counter-- > 0) {
	}

	fclose(fp2);
	fclose(fp3);
	unlink("sample");

	return 1;
}
