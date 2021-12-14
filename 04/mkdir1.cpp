#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <queue>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <errno.h>

using namespace std;

enum ParentOptions { PARENTS, NONE };
unordered_map<string, ParentOptions> rawCommand{{"-p", ParentOptions::PARENTS}};

ParentOptions getParentOption(queue<string>& commands)
{
	int size = commands.size();
	unordered_map<string, ParentOptions>::iterator iter;
	while (size)
	{
		string curItem = commands.front();
		iter = rawCommand.find(curItem);
		ParentOptions tmp = (iter == rawCommand.end()) ? ParentOptions::NONE : iter->second;
		commands.pop();
		if (tmp != ParentOptions::NONE) return tmp;
		commands.push(curItem);
		size--;
	}
	return ParentOptions::NONE;
}

void createDirWithoutOptions(const string& dirname)
{
	if (mkdir(dirname.c_str(), 0644) == -1)
	{
		cerr << "create directory '" << dirname << "' error" << endl;
		return;
	}
}

void createDirWithParents(const string& path)
{
	//cout << "path:" << path << '\t';
	stringstream ss{path};
	string tmp;
	queue<string> dirnames;
	while (getline(ss, tmp, '/'))
		dirnames.push(tmp);
	while (dirnames.size() > 1)
	{
		string curDir = dirnames.front();
		dirnames.pop();
		if (curDir == "") 
		{
			if (chdir("/") == -1)
			{
				cerr << "Change to root directory error" << endl;
				return;
			}
			continue;
		}
		struct stat buf;
		if (stat(curDir.c_str(), &buf) == -1)
		{
			if (errno == ENOENT && !curDir.empty())
			{
				createDirWithParents(curDir);
				if (chdir(curDir.c_str()) == -1)
				{
					cerr << "Change to directory '" << tmp << "' error before creating directory" << endl;
					return;
				}
			}
			else
			{
				cerr << "The directory '" << tmp << "' is not exist" << endl;
				return;
			}
		}
		else if (S_ISDIR(buf.st_mode)) 
		{
			if (chdir(curDir.c_str()) == -1)
			{
				cerr << "Change to directory '" << tmp << "' error" << endl;
				return;
			}
		}
	}
	createDirWithoutOptions(dirnames.front());
}

int main(int ac, char* argv[])
{
	if (ac == 1)
	{
		cerr << "mkdir1: missing operand" << endl;
		return -1;
	}
	queue<string> commandInPiece;
	while (ac)
	{
		ac--;
		commandInPiece.push(argv[ac]);
	}
	ParentOptions optionInCommand = getParentOption(commandInPiece);	
	switch (optionInCommand)
	{
		case ParentOptions::PARENTS:
			createDirWithParents(commandInPiece.front());
			break;
		case ParentOptions::NONE:
			createDirWithoutOptions(commandInPiece.front());
			break;
		default:
			break;
	}
	return 0;
}

