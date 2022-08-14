#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


static int sigCnt[NSIG];

static sig_atomic_t gotSigint = 0;

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
	int sig, cnt;

	cnt = 0;
	for (sig = 1; sig < NSIG; sig++) {
		if (sigismember(sigset, sig)) {
			cnt++;
			fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
		}
	}

	if (cnt == 0) {
		fprintf(of, "%s<empty signal set>\n", prefix);
	}
}


static void handler(int sig)
{
	if (sig == SIGINT)
		gotSigint = 1;
	else
		sigCnt[sig]++;
}


int main(int argc, char *argv[])
{
	int n, numSecs;
	sigset_t pendingMask, blockingMask, emptyMask;

	printf("%s: pid is %d\n", argv[0], getpid());

	for (n = 1; n < NSIG; n++) {
		signal(n, handler);
	}

	if (argc > 1) {
		//sscanf(&numSecs, "%d", argv[1]);
		numSecs = atoi(argv[1]);
		
		sigfillset(&blockingMask);
		if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1) {
			perror("sigpromask fail.\n");
			return -1;
		}

		printf("%s: sleeping for %d seconds.\n", argv[0], numSecs);
		sleep(numSecs);

		if (sigpending(&pendingMask) == -1) {
			perror("sigpending() ");
			return -1;
		}

		printf("%s: pending signals are:\n", argv[0]);
		printSigset(stdout, "\t\t", &pendingMask);

		sigemptyset(&emptyMask);
		if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1) {
			perror("sigpromask() error");
			return -1;
		}
	}

	while (!gotSigint) {
		continue;
	}

	for (n = 1; n < NSIG; n++) {
		if (sigCnt[n] != 0) {
			printf("%s: signal %d caught %d times\n", argv[0], 
							n, sigCnt[n]);
		}
	}

	return 0;
}








