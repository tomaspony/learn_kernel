#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<string.h>


int main(int argc, char *argv[])
{
	char *addr;

	addr = mmap(NULL, sizeof(char) * 20, PROT_READ | PROT_WRITE,
				MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap failed");
		return -1;
	}

	//*addr = 1;

	switch (fork()) {
	case -1:
		perror("fork error!");
		return -1;
	case 0:
		printf("child started, value=%s\n", addr);
		strncpy(addr, "hello,world!", 19);
		//(*addr)++;
		break;
	default:
		if (wait(NULL) == -1) {
			perror("wait error!");
			return -1;
		}

		printf("in parent, value = %s\n", addr);
		if (munmap(addr, sizeof(char) * 20) == -1) {
			perror("munmap error!");
			return -1;
		}
		return 0;
	}
}


















