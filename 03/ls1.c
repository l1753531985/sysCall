#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char*);

int main(int ac, char* argv[])
{
	//printf("argv: %s\n", argv[1]);
	if (ac == 1)
		do_ls(".");
	else
		while (--ac)
		{
			printf("%s:\n",*++argv);
			do_ls(*argv);
		}
	return 0;
}

void do_ls(char* dirname)
{
	DIR* dir_ptr;
	struct dirent* direntp; 

	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		while ((direntp = readdir(dir_ptr)) != NULL)
			printf("%s\n", direntp->d_name); 
		closedir(dir_ptr);
	}
}
