#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int numSigs, sig, j;
	pid_t pid;

	//sscanf(&pid, "%d", argv[1]);
	//sscanf(&numSigs, "%d", argv[2]);
	//sscanf(&sig, "%d", argv[3]);
	pid = atoi(argv[1]);
	numSigs = atoi(argv[2]);
	sig = atoi(argv[3]);

	printf("pid = %d, numSigs=%d, sig=%d\n", pid, numSigs, sig);
	/* send signals to receiver */
	printf("%s: sending signal %d to process %ld %d times\n",
			argv[0], sig, (long)pid, numSigs);
	for (j = 0; j < numSigs; j++) {
		if (kill(pid, sig) == -1) {
			perror("kill failed");
			return -1;
		}
	}

	if (argc > 4) {
		int sig2;
		//sscanf(&sig2, "%d", argv[4]);
		sig2 = atoi(argv[4]);
		if (kill(pid, sig2) == -1) {
			perror("kill failed.");
			return -1;
		}
	}

	printf("%s:exiting.\n", argv[0]);
	return 0;
}







