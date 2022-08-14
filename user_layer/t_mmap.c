#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<string.h>


#define MEM_SIZE 10

int main(int argc, char *argv[])
{
	char *addr;
	int fd;

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("open error\n");
		return -1;
	}

	addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap failed\n");
		return -1;
	}

	close(fd);
	printf("current string=%.*s\n", MEM_SIZE, addr);

	if (argc >= 2) {
		if( strlen(argv[2]) >= MEM_SIZE) {
			perror("new-value too large\n");
			return -1;
		}

		memset(addr, 0, MEM_SIZE);
		strncpy(addr, argv[2], MEM_SIZE -1);
		if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
			perror("msync error!");
			return -1;
		}
		printf("Copied \"%s\" to shared memory\n", argv[2]);
	}

	return 0;
}












