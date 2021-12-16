#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <time.h>
#include <unordered_map>
#include <errno.h>

using namespace std;

struct OptionType {
	enum SortOption {ORDER, DORDER, UNORDER};
	enum RecursionOption {RECURSION, NONRECURSION};
	enum TimeOption {LASTACCESS, CREATEFILE};
};

enum OptionClass {SORTCLASS, RECURSIONCLASS, TIMECLASS};

unordered_map<string, OptionType::SortOption> sortOptions{{"-r", OptionType::DORDER}, {"-q", OptionType::UNORDER}};
unordered_map<string, OptionType::RecursionOption> recursionOptions{{"-R", OptionType::RECURSION}};
unordered_map<string, OptionType::TimeOption> timeOptions{{"-u", OptionType::LASTACCESS}};	

struct OptionStatus {
	OptionType::SortOption sortItem = OptionType::ORDER;
	OptionType::RecursionOption recursionItem = OptionType::NONRECURSION;
	OptionType::TimeOption timeItem = OptionType::CREATEFILE;
};

unordered_map<string, OptionClass> allOptions{{"-r", OptionClass::SORTCLASS}, {"-q", OptionClass::SORTCLASS}, {"-R", OptionClass::RECURSIONCLASS}, {"-u", OptionClass::TIMECLASS}};

void do_ls(const string&, const OptionStatus&);
void do_stat(const string&);
void show_file_info(const string&, struct stat*);
void mode_to_letter(mode_t, string&);
string uid_to_name(uid_t);
string gid_to_name(gid_t);

template <typename Item> 
void sortElem(vector<Item>& vi, const OptionType::SortOption& sortOption)
{
	switch (sortOption)
	{
		case OptionType::ORDER:
			sort(vi.begin(), vi.end(), less<Item>());
			break;
		case OptionType::DORDER:
			sort(vi.begin(), vi.end(), greater<Item>());
			break;
		default:
			break;
	}
}

void getDirnameAndOptions(const string& para, queue<string>& dirnames, OptionStatus& stu)
{
	unordered_map<string, OptionClass>::iterator iter = allOptions.find(para);
	if (iter == allOptions.end())
		dirnames.push(para);	
	else 
	{
		unordered_map<string, OptionType::SortOption>::iterator sortIter = sortOptions.find(para);
		unordered_map<string, OptionType::RecursionOption>::iterator recursionIter = recursionOptions.find(para);
		unordered_map<string, OptionType::TimeOption>::iterator timeIter = timeOptions.find(para);
		switch (iter->second)
		{
			case OptionClass::SORTCLASS:
				stu.sortItem = (sortIter == sortOptions.end()) ? OptionType::ORDER : sortIter->second; 
				break;
			case OptionClass::RECURSIONCLASS:
				stu.recursionItem = (recursionIter == recursionOptions.end()) ? OptionType::NONRECURSION : recursionIter->second; 
				break;
			case OptionClass::TIMECLASS:
				stu.timeItem = (timeIter == timeOptions.end()) ? OptionType::CREATEFILE : timeIter->second;
				break;
			default:
				break;
		}
	}
}

int main(int ac, char* argv[])
{
	queue<string> dirnames;
	OptionStatus stu;

	while (--ac)
	{
		++argv;
		getDirnameAndOptions(*argv, dirnames, stu);
	}	
	
	if (dirnames.empty()) dirnames.push(".");
	while (!dirnames.empty())
	{
		do_ls(dirnames.front(), stu);
		dirnames.pop();
	}
	return 0;
}

void do_ls(const string& dirname, const OptionStatus& stu)
{
	cout << dirname << endl;
	if (chdir(dirname.c_str()) == -1) perror(dirname.c_str());
	DIR* dir_ptr = opendir(dirname.c_str());	
	struct dirent* direntp = nullptr;
	if (!dir_ptr)
	{
	       	cerr << "Cannot open: " << endl;
		if (errno == EACCES)
			cout << "you don't have permission to open file" << endl;
		else if (errno == EBADF)
			cout << "fd is not a vaild file descrioptor" << endl;
		else if (errno == EMFILE)
			cout << "the system-wide limit" << endl;
		else if (errno == ENOENT)
			cout << "directory does not exist" << endl;
		else if (errno == ENOMEM)
			cout << "insufficient memory to complete the operation" << endl;
		else if (errno == ENOTDIR)
			cout << "name is not a directory" << endl;
		else 
			cout << "unknow errors" << endl;

	}
	else
	{
		vector<string> d_names;
		while (direntp = readdir(dir_ptr))
			d_names.push_back(direntp->d_name);
		sortElem(d_names, stu.sortItem);
		for (string name : d_names)
			do_stat(name);
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
	cout << info_p->st_ino << " ";
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
