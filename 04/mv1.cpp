#include <iostream>
#include <stack>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>

using namespace std;

void renameFile(const string& oldPath, const string& newPath)
{
	struct stat infoOld, infoNew;
	if (stat(newPath.c_str(), &infoNew) == -1 || stat(oldPath.c_str(), &infoOld) == -1)
	{
		cerr << "error occured during getting infomaiton of " << newPath << " or " << oldPath << endl;
		return;
	}
	string fileName;
	if (S_ISDIR(infoNew.st_mode))
	{
		stringstream ss{oldPath};
		string name;
		stack<string> namesPools;
		while (getline(ss, name, '/'))
			namesPools.push(name);
		fileName = namesPools.top();
	}
	fileName = newPath + fileName;
	cout << oldPath << endl;
	cout << fileName << endl;
	if (rename(oldPath.c_str(), fileName.c_str()) == -1)
	{
		cerr << "rename error occurd!" << endl;
		return;
	}
}

int main(int ac, char* argv[])
{
	struct stat infobuf;
	switch (ac)
	{
		case 1:
			cerr << "missing file operand" << endl;
			return -1;
		case 2:
			cerr << "missing destination file operand after '" << argv[1] << "'" << endl;
			return -1;
		default:
			if (stat(argv[1], &infobuf) == -1)
			{
				cerr << "Cannot get information about '" << argv[1] << endl;
				return -1;
			}
			if (!S_ISREG(infobuf.st_mode)) 
			{
				cerr << argv[1] << "is not a reguler file" << endl;
				return -1;
			}
			renameFile(argv[1], argv[2]);
			break;
	}
	return 0;
}
