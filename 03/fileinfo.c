#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void show_stat_info(char*, struct stat*);

int main(int ac, char* argv[])
{
	//printf("argv: %s\n", argv[1]);
	struct stat info;
	return 1;
}

void show_stat_info(char* fname, struct stat* buf)
{
	printf("	mode: %o\n", buf->st_mode);
	printf("	links: %d\n", buf->st_nlink);
	printf("	user: %d\n", buf->st_uid);
	printf("	group: %d\n", buf->st_gid);
	printf("	size: %d\n", buf->st_size);
	printf("	modtime: %d\n", buf->st_mtim);
	printf("	name: %s\n", fname);
}

