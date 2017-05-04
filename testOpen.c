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
	fclose(fp2);
	unlink("sample");

	return 1;
}
