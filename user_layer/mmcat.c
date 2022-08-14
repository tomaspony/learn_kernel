#include<stdlib.h>
#include<stdio.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	char *addr;
	int fd;
	struct stat sb;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open error!\n");
		return -1;
	}

	if (fstat(fd, &sb) == -1) {
		perror("fstat error!\n");
		return -1;
	}

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap error\n");
		return -1;
	}

	close(fd);
	
	if (write(1, addr, sb.st_size) != sb.st_size) {
		perror("partial write\n");
		return -1;
	}


	return 0;
}















