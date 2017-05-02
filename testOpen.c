#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *fp;
	fp = fopen("note", "r");
	if (fp) {
		fclose(fp);
	}
	return 1;
}
