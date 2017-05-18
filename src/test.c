#include <sys/stat.h>
#include <unistd.h>

//small program that does a different syscall depending on if there were command line arg's or not
void main(int argc, char **argv) {
	if (argc != 1)
		mkdir("trial", 0777);
	else
		rmdir("trial");
}
