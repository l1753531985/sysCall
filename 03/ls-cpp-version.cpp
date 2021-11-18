#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <time.h>

using namespace std;

void do_ls(const string&);
void do_stat(const string&);
void show_file_info(const string&, struct stat*);
void mode_to_letter(mode_t, string&);
string uid_to_name(uid_t);
string gid_to_name(gid_t);

int main(int ac, char* argv[])
{
	if (ac == 1)
		do_ls(".");
	else
		while (--ac)
		{
			++argv;
			cout << *argv << endl;
			//if (chdir(*argv) == -1)
			//	perror(*argv);
			//else
				do_ls(*argv);
		}	
	return 0;
}

void do_ls(const string& dirname)
{
	// test
	//cout << "dirname is " << dirname << endl;
	DIR* dir_ptr = opendir(dirname.c_str());	
	struct dirent* direntp = nullptr;

	if (!dir_ptr) cerr << "ls1: cannot open " << dirname << endl;
	else
	{
		while (direntp = readdir(dir_ptr))
			do_stat(direntp->d_name);
		if (closedir(dir_ptr) == -1)
			perror(dirname.c_str());
	}
}

void do_stat(const string& filename)
{
	struct stat info;
	if (stat(filename.c_str(), &info) == -1) 
		perror(filename.c_str());
	else
		show_file_info(filename, &info);
}

void show_file_info(const string& filename, struct stat* info_p)
{
	//const string& uid_to_name(uid_t);	
	//const string& gid_to_name(gid_t);	
	string modestr = "----------";

	mode_to_letter(info_p->st_mode, modestr);

	cout << modestr;
	cout << right << setw(4) << info_p->st_nlink << " "; 
	cout << left << setw(8) << uid_to_name(info_p->st_uid);
	cout << left << setw(8) << gid_to_name(info_p->st_gid);
	cout << right << setw(8) << (long)info_p->st_size;
	printf("%13.12s", 4+ctime(&(info_p->st_mtime)));
	cout << " " << filename << endl;
}

void mode_to_letter(mode_t mode, string& str)
{
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

string uid_to_name(uid_t uid)
{
	struct passwd* pw_ptr = nullptr;
	struct passwd* getpwuid(uid_t uid);
	return (pw_ptr = getpwuid(uid)) ? pw_ptr->pw_name : to_string(uid);
}

string gid_to_name(gid_t gid)
{
	struct group* grp_ptr;
	struct group* getgrgid(gid_t);
	return (grp_ptr = getgrgid(gid)) ? grp_ptr->gr_name : to_string(gid);
}
