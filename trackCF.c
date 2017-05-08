#include "trackCF.h"

registers * registersHead = NULL;
files * filesHead = NULL;

//print all the records to a file
//format: start, syscall, orig_eax, eax, ebx, ecx, edx, esi, edi
//the function that calls this must make sure the FILE * passed must be open before calling and closed afterwards
void printRegisterRecords(FILE * output) {
	registers * temp = registersHead;
	while (temp != NULL) {
		fprintf(output, "%d %ld %ld %ld %ld %ld %ld %ld\n", temp->start, temp->orig_eax, temp->eax, temp->ebx, temp->ecx, temp->edx, temp->esi, temp->edi);
		temp = temp->next;
	}
}

//prints all file touches to a file
//format: syscall, name1, name2
//the function that calls this must make sure the FILE * passed must be open before calling and closed afterwards
void printFileRecords(FILE * output) {
	files * temp = filesHead;
	while (temp != NULL) {
		fprintf(output, "%ld %s %s\n", temp->syscall, temp->name1, temp->name2);
		temp = temp->next;
	}
}

//add to the registers list. 1 is failure, 0 is success
int addRegisterRecord(int h, long a, long b, long c, long d, long e, long f, long g) {
	registers * element;
	if ((element = malloc(sizeof(registers))) == NULL)
		return 1;
	element->start = h;
	element->orig_eax = a;
	element->eax = b;
	element->ebx = c;
	element->ecx = d;
	element->edx = e;
	element->esi = f;
	element->edi = g;
	element->next = NULL;
	if (registersHead == NULL)
		registersHead = element;
	else {
		registers * temp = registersHead;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = element;
	}
	return 0;
}

//add to the files list. 1 is failures, 0 is success
int addFileRecord(long a, char * b, char * c) {
	files * element;
	if ((element = malloc(sizeof(files))) == NULL)
		return 1;
	element->syscall = a;
	element->name1 = b;
	element->name2 = c;
	element->next = NULL;
	if (filesHead == NULL)
		filesHead = element;
	else {
		files * temp = filesHead;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = element;
	}
	return 0;
}

//free all allocated memory
void freeAll() {
	registers * temp = registersHead;
	while (temp != NULL) {
		registers * temp2 = temp->next;
		//free the struct
		free(temp);
		temp = temp2;
	}
	files * temp2 = filesHead;
	while (temp2 != NULL) {
		//free the allocated strings
		free(temp2->name1);
		free(temp2->name2);
		files * temp3 = temp2->next;
		//then free the struct
		free(temp2);
		temp2 = temp3;
	}
}

//print contents of frequency table
void printCalls(int * array, int max) {
	int counter = 0;
	printf("\nSyscall\t|\tFrequency\n");
	for(; counter < max; counter++) {
		if (array[counter] > 0)
			printf("%d\t:\t%d times\n", counter, array[counter]);
	}
}

//checks for occurrence of null character within length of start
int findNull(char * start, int length) {
	int counter = 0;
	for(; counter < length; counter++) {
		if (start[counter] == '\0')
			return 1;
	}
	return 0;
}

//this is needed because the data at address resides in the child process; the parent process cannot normally
//access this data
//given the start address and the pid of the child process, get the string at that address
//the pointer returned by this must be freed 
void * getString(pid_t child, long address) {
	int foundNull = 0;
	int initSize = 32;
	void * ret;
	if ((ret = malloc(initSize)) == NULL)
		return NULL;
	long counter = 0;
	int checkedSize = 0;
	while(!foundNull) {
		//get the data. ptrace only returns words, so need to grab data a word at a time
		long data = ptrace(PTRACE_PEEKDATA, child, address + sizeof(long) * counter, NULL);
		//copy the data to the allocated section
		memcpy(ret + sizeof(long) * counter, &data, sizeof(long));
		//check if the end of the string (a null byte) has been found
		if (!findNull(ret, sizeof(long) * (counter+1))) {
			//if not then update var's and continue
			counter++;
			checkedSize += 4;
		}
		else
			break;
		//is the string buffer maxed out?
		if (checkedSize == initSize) {
			initSize *= 2;
			void * temp;
			if ((temp = realloc(ret, initSize)) == NULL) {
				free(ret);
				return NULL;
			}
			else {
				ret = temp;
			}
		}
	}
	return ret;
}

//retrieve the number of digits in a number
int getLength(int number) {
	int counter = 0;
	for(; number!=0; number /= 10) {counter++;}
	return counter;
}

//append a char to the end of a string. return 1 (failure) if unable to, else 0
int append(char * s, int size, char c) {
	if (strlen(s) + 1 >= size)
		return 1;
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return 0;
}

//this runs a bash script that will find the filename
//given a file descriptor, gets the file name associated with it
//the pointer to this must be freed
void * getFileName(unsigned int fd, pid_t child) {
	//get the lengths of the integers to allocate the appropriate amount
	int fdLength = getLength(fd);
	int pidLength = getLength(child);
	//now build the command
	char * command;
	if ((command = calloc(17 + fdLength + pidLength, 1)) == NULL)
		return NULL;
	sprintf(command, "./findFileName %u %u", child, fd);
	
	FILE * fp = popen(command, "r");
	int size = 50;
	char * result;
	if ((result = calloc(size, 1)) == NULL) {
		free(command);
		return NULL;
	}
	char c;
	do {
		c = fgetc(fp);
		if ((c == '\0') || (c == '\n'))
			break;
		else {
			if (strlen(result) + 1 >= size) {
				size *= 1.2;
				char * temp;
				if ((temp = realloc(result, size)) == NULL) {
					free(result);
					free(command);
					return NULL;
				}
				else {
					result = temp;
				}
			}
			else {
				append(result, size, c);
			}
		}
	}while (!feof(fp));
	
	free(command);
	return result;
}

//if malloc fails, should exit. this function frees all previously allocated memory, kills children, and exits
void exitGracefully(int child) {
	freeAll();
	kill(child, SIGKILL);
	printf("Ran out of memory!\n");
	exit(0);
}

int main() {
	pid_t child;	
	//keep track of all syscalls
	int countCalls[338] = {0};
	child = fork();
	if (child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);		

//		execl("/bin/ls", "ls", NULL);
//		execl("/usr/bin/cal", "cal", NULL);
//		execl("/usr/bin/ncal", "ncal", NULL);
//		execl("/bin/date", "date", NULL);
//		execl("/bin/pwd", "pwd", NULL);
//		execl("/usr/bin/time", "time", NULL);
//		execl("/usr/bin/head", "head", "sampletext", NULL);
//		execl("/bin/cat", "cat", "sampletext", NULL);
//		execl("/bin/chmod", "chmod", "+r", "note", NULL);
		execl("/home/sekar/Desktop/360/testOpen", "./testOpen", NULL);
	}
	else {
		struct user_regs_struct regs;
		int status = 0;
		//PTRACE_SYSCALL will force the child process to stop at the beginning and end of a syscall
		//need to distinguish between the two
		while(waitpid(child, &status, 0) && (!WIFEXITED(status))) {
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			printf("orig_eax: %ld eax:%ld ebx: %ld ecx: %ld edx: %ld esi: %ld edi: %ld\n", regs.orig_eax, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi);
			countCalls[regs.orig_eax]++;
			//IDEA: This .c file will be the dry run. It will record all syscalls and file I/O
			//and put this into a log file. The wet run could be a separate program that reads
			//this log file.
			
			//log the syscall and the registers
			//are we at the start or end of a syscall? ignore call 11 and 252
			if ((regs.orig_eax == 11) || (regs.orig_eax == 252))
				addRegisterRecord(2, regs.orig_eax, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi);
			//if eax is -38, then we are at start
			else if (regs.eax == -38) {
				addRegisterRecord(1, regs.orig_eax, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi);
			}
			else {
				addRegisterRecord(0, regs.orig_eax, regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esi, regs.edi);
			}
			
			//next need to log potential files. each file related syscall has var's in diff registers. 
			
			char * oldFileName;
			char * newFileName;
			int fileIO = 0;
			switch(regs.orig_eax) {
				//the open, create or unlink syscall
				case 5:
				case 8:
				case 10:
					if ((oldFileName = getString(child, regs.ebx)) == NULL) {
						exitGracefully(child);
					}
					newFileName = NULL;
					fileIO = 1;					
					break;
				//the openat or unlinkat syscall
				case 295:
				case 301:
					if ((oldFileName = getString(child, regs.ecx)) == NULL) {
						exitGracefully(child);
					}
					newFileName = NULL;
					fileIO = 1;
					break;
				//the read, readv, preadv, write, writev, pwritev, or close syscall
				case 3:
				case 4:
				case 6:
				case 145:
				case 146:
				case 333:
				case 334:
					if ((oldFileName = getFileName(regs.ebx, child)) == NULL) {
						exitGracefully(child);
					}
					newFileName = NULL;
					fileIO = 1;
					break;
				//the rename, link, or symlink syscall
				case 9:
				case 38:
				case 83:
					if ((oldFileName = getString(child, regs.ebx)) == NULL) {
						exitGracefully(child);
					}
					if ((newFileName = getString(child, regs.ecx)) == NULL) {
						exitGracefully(child);
					}
					fileIO = 1;
					break;
				//the renameat or linkat syscall
				case 302:
				case 303:
					if ((oldFileName = getString(child, regs.ecx)) == NULL) {
						exitGracefully(child);
					}
					if ((newFileName = getString(child, regs.esi)) == NULL) {
						exitGracefully(child);
					}
					fileIO = 1;
					break;
				//the symlinkat syscall
				case 304:
					if ((oldFileName = getString(child, regs.ebx)) == NULL) {
						exitGracefully(child);
					}
					if ((newFileName = getString(child, regs.edx)) == NULL) {
						exitGracefully(child);
					}
					fileIO = 1;
					break;
				default:
					break;
			}
			//should only log before the syscall, not after
			if ((regs.eax == -38) && (fileIO))
				addFileRecord(regs.orig_eax, oldFileName, newFileName);
			

			ptrace(PTRACE_SYSCALL, child, 0, 0);
		}

	}
	//finally write all data to files
	FILE * registersFile = fopen("registerRecords", "w+");
	FILE * fileHistory = fopen("fileHistory", "w+");

	printRegisterRecords(registersFile);
	printFileRecords(fileHistory);
	fclose(registersFile);
	fclose(fileHistory);

	printCalls(countCalls, 338);
	freeAll();
	return 0;
}
