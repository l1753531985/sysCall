#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

 int main(int ac, char* argv[])
{
	// get tty name and stat
	string ttypath = ttyname(0);
	struct stat statbuf;
	if (stat(ttypath.c_str(), &statbuf) == -1)
	{
		perror(ttypath.c_str());
		exit(1);
	}
        // mesg command operate
	int answer_int = statbuf.st_mode & S_IWGRP;
	string answer = (answer_int) ? "is y" : "is n";
	if (ac == 1)
	{
	       	cout << answer << endl;
		return 0;
	}

	string option = argv[1];
	int ret = -1;
	if (option[0] == 'y' || option[0] == 'Y')
		ret = chmod(ttypath.c_str(), 00620);
	else
		ret = chmod(ttypath.c_str(), 00600);
	if (ret == -1)
	{
		perror(ttypath.c_str());
		exit(1);
	}
	return 0;
}
