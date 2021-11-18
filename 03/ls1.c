#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>

void do_ls(char*);
char* gid_to_name(gid_t);
char* uid_to_name(uid_t);
void mode_to_letter(mode_t, char*);
void do_stat(char*); 
void show_file_info(char*, struct stat*);

int main(int ac, char* argv[])
{
	//printf("argv: %s\n", argv[1]);
	if (ac == 1)
		do_ls(".");
	else
		while (--ac)
		{
			printf("%s:\n",*++argv);
			if (chdir(*argv) == -1)
				perror(*argv);
			else
				do_ls(*argv);
		}
	return 0;
}


void do_ls(char dirname[])
{
	DIR* dir_ptr;
	struct dirent* direntp; 

	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		while ((direntp = readdir(dir_ptr)) != NULL)
			do_stat(direntp->d_name);
		closedir(dir_ptr);
	}
}

void do_stat(char* filename) 
{
	struct stat info;

	if (stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char* filename, struct stat* info_p)
{
	char *uid_to_name(uid_t), *gid_to_name(gid_t gid);
	char modestr[11];

	mode_to_letter(info_p->st_mode, modestr);

	printf("%s", modestr);
	printf("%4d ", (int)info_p->st_nlink);
	printf("%-8s", uid_to_name(info_p->st_uid));
	printf("%-8s", gid_to_name(info_p->st_gid));
	printf("%8ld", (long)info_p->st_size);
	printf("%13.12s", 4+ctime(&info_p->st_mtime));
	printf(" %s\n", filename);
}

void mode_to_letter(mode_t mode, char str[])
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';
	if (mode & S_ISUID) str[3] = 'S';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';
	if (mode & S_ISGID) str[6] = 'S';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
	if (mode & S_ISVTX) str[9] = 'T';
}

char* uid_to_name(uid_t uid)
{
	struct passwd *pw_ptr, *getpwuid(uid_t uid);
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else	
		return pw_ptr->pw_name;
}

char* gid_to_name(gid_t gid)
{
	struct group *grp_ptr, *getgrgid(gid_t);
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL)
	{
		sprintf(numstr, "%d", gid);
		return numstr;
	} 
	else
		return grp_ptr->gr_name;
}
