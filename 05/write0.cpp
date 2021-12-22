#include <iostream>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utmp.h>

using namespace std;

void getUserTTY(unordered_map<string, vector<string>>& ttyOfUser)
{
	int fd = open("/var/run/utmp", O_RDONLY); 	
	if (fd == -1)
	{
		cerr << "openning utmp file is error" << endl;
		exit(1);
	}
	struct utmp utmpbuf;
	const int UTSIZE = sizeof(struct utmp);
	while (read(fd, &utmpbuf, UTSIZE) > 0)
		if (utmpbuf.ut_type == USER_PROCESS)
		{
			unordered_map<string, vector<string>>::iterator iter = ttyOfUser.find(utmpbuf.ut_user);
			if (iter == ttyOfUser.end())
			{
				vector<string> ttys;
				ttyOfUser.insert({utmpbuf.ut_user, ttys});
			}
			ttyOfUser[utmpbuf.ut_user].push_back(utmpbuf.ut_line);
		}
	if (close(fd) == -1)
	{
		cerr << "closing utmp file is error" << endl;
		exit(1);
	}
}

void showSingleUserTTY(const vector<string>& ttys)
{
	for (string s : ttys)
		cout << "/dev/" << s << " ";
	cout << endl;
}

void showUserTTY(unordered_map<string, vector<string>>& userToTTY, const string& name)
{
	if (name == "") return;
	if (name == "anyone")
	{
		unordered_map<string, vector<string>>::iterator iter = userToTTY.begin();
		for (; iter != userToTTY.end(); iter++)
		{
			cout << iter->first << ": ";
			showSingleUserTTY(iter->second);
		}
	}
	else if (name == "root")
	{
		vector<string> ttys = userToTTY[name];
		if (ttys.empty()) cout << "vector is emtpy" << endl;
		showSingleUserTTY(ttys);
	}
}

int main(int ac, char* argv[])
{
	unordered_map<string, vector<string>> userToTTY;
	getUserTTY(userToTTY);
	string username = "";
	if (ac == 1) username = "anyone";
	else if (ac == 2) username = argv[1];
	showUserTTY(userToTTY, username);
	/*
	if (ac != 2) 
	{
		cerr << "usage: write0 ttyname" << endl;
		return -1;
	}
	*/
	if (ac != 3) return -1;
	int fd = open(argv[2], O_WRONLY);
	if (fd == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	string line;
	while (getline(cin, line))
	{
		line += "\n";
		if (write(fd, line.c_str(), line.size()) == -1)
			break;
	}
	if (close(fd) == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	return 0;
}
