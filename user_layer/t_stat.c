#define __DEFAULE_SOURCE 
#include<stdio.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/types.h>
#include<stdlib.h>


static void display_stat_info(const struct stat *sb)
{
	printf("File type:		");
	switch (sb->st_mode & S_IFMT) {
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFCHR:
		printf("charactere device\n");
		break;
	case S_IFBLK:
		printf("block device");
		break;
	case S_IFLNK:
		printf("symbolic soft link\n");
		break;
	case S_IFIFO:
		printf("FIFO or pipe\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown file type\n");
	}

	//printf("Device containing i-node: major=%ld minor=%ld\n",
	//		(long)major(sb->st_dev), (long)minor(sb->st_dev));

	printf("I-node number:		%ld\n", (long)sb->st_ino);

	printf("Mode:		%lo\n", (unsigned long)sb->st_mode);

	if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX)) {
		printf(" special bits set:	%s%s%s\n",
			(sb->st_mode & S_ISUID) ? "set-UID": "",
			(sb->st_mode & S_ISGID) ? "set-GID": "",
			(sb->st_mode & S_ISVTX) ? "sticky" :"");
	}

	printf("Number of hard links: %ld\n", (long)sb->st_nlink);
	
	printf("Ownership:		UID=%ld GID=%ld\n",
			(long)sb->st_uid, (long)sb->st_gid);

	printf("File Size:		%lld bytes\n", (long long)sb->st_size);
	printf("Optimal I/O block size: %ld bytes\n", (long)sb->st_blksize);
	printf("512B blocks allocated:	%lld\n", (long long)sb->st_blocks);

	printf("last file access:		%s\n", ctime(&sb->st_atime));
	printf("last file modification:	%s\n", ctime(&sb->st_mtime));
	printf("last status change:		%s\n", ctime(&sb->st_ctime));
}


int main(int argc, char *argv[])
{
	struct stat sb;

	if (stat(argv[1], &sb) == -1) {
		exit(-1);
	}

	display_stat_info(&sb);

	return 0;
}





















