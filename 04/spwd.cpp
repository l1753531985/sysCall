#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

ino_t get_inode(const string& fname)
/*
 * returns inode number of the file
 */
{
	struct stat info;
	if (stat(fname.c_str(), &info) == -1)
	{
		cerr << "Cannot stat " << fname << endl;
		exit(1);
	}
	return info.st_ino;
}

void inum_to_name(ino_t inode_to_find, string& nameBuf)
{
	DIR* dir_ptr = opendir(".");
	if (!dir_ptr)
	{
		cerr << "Cannot opendir ." << endl;
		exit(1);
	}
	struct dirent* direntp = nullptr;
       	while ((direntp = readdir(dir_ptr)))
	{
		if (direntp->d_ino == inode_to_find)
		{
			nameBuf = direntp->d_name;
			closedir(dir_ptr);
			return;
		}
	}
	cerr << "error looking for inum " << inode_to_find << endl;
	exit(1);
}

void printPathTo(ino_t this_inode)
/*
 * prints path leading down to an object with this inode
 * kindof recursive
 */
{
	if (get_inode("..") != this_inode)	
	{
		string its_name;
		chdir("..");
		inum_to_name(this_inode, its_name);
		printPathTo(get_inode("."));
		cout << "/" << its_name;
	}
}

int main()
{
	//test
	const string testName = ".";
	printPathTo(get_inode("."));
	cout << endl;
	return 0;
}

