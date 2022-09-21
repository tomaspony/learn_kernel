#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>


#define DEVICE "/dev/second"
#define MEM_CLEAR 0x1

int main()
{
	int fd;
	int count = 0, old_count = 0;
	int ret;

	fd = open("/dev/second", O_RDONLY);
	if (fd == -1) {
		printf("open file failed\n");
		return -1;
	}

	while (1) {
		ret = read(fd, &count, sizeof(int));
		if (ret == -1) {
			printf("read failed\n");
			return -1;
		}

		if (count != old_count) {
			printf("seconds after open /dev/second:%d\n", count);
			old_count = count;
		}
	}
	
	
	close (fd);
	
	return 0;
}




















