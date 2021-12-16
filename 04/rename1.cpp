#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

using namespace std;

int moveFileInode(const string& oldpath, const string& newpath)
{
	struct stat info;
	if (stat(oldpath.c_str(), &info) == -1)
	{
		cerr << "Getting info of " << oldpath << "occured erroring" << endl;
		return -1;
	}
	if (link(oldpath.c_str(), newpath.c_str()) != -1)
		if (unlink(oldpath.c_str()) == -1)
			cerr << "unlink " << oldpath << "error" << endl;
}

int main(int ac, char* argv[])
{
	if (ac != 3) 
	{
		cerr << "missing operate!" << endl;
		return -1;
	}	
	string fileNameNewpath = argv[2];
	string fileNameOldpath = argv[1];
	if (moveFileInode(fileNameOldpath, fileNameNewpath) == -1)
	/*
	if (link("x", "z") != -1)
		unlink("x");
	*/
	return 0;
}
