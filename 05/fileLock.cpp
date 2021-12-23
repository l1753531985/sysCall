#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

using namespace std;

/*
 * return 0 if ok, 1 if already locked, 2 if other problem
 * link will not overwrite newpath, if newpath is already existing
 */
bool lockFile(const string& filename)
{
	int rv = true;
	string lockfile = filename + ".LCK";
	if (link(filename.c_str(), lockfile.c_str()) == -1)
	{
		rv = (errno == EEXIST) ? true : false;
		cout << "errno exist: " << EEXIST << endl;
	}
	return rv;
}

int unlockFile(const string& filename)
{
	int rv = 0;
	if (unlink((filename+".LCK").c_str()) == -1)
	{
		rv = -2;
		if (errno == ENOENT) rv = 0;
		if (errno == EBUSY)  rv = -1;
	}
	return rv;
}

void writeMessageToFile(const string& target, const string& mesg)
{
	int fd = open(target.c_str(), O_WRONLY);
	if (fd == -1)
	{
		perror(target.c_str());
		exit(1);
	}
	int size = sizeof(mesg.c_str());
	if (write(fd, mesg.c_str(), size) != size)
	{
		perror(target.c_str());
		exit(1);
	}
	sleep(20);
	if (close(fd) == -1)
	{
		perror(target.c_str());
		exit(1);
	}
}

int writeMessageInOrder(const string& target, const string& mesg, int times)
{
	while (times--)
	{
		cout << "file: " << mesg << " in while" << endl;
		int ret = unlockFile(target);
		if (ret < 0)
		{
			if (ret == -1) 
				cout << "The file " << target << "is busy" << endl;  
			else
				cout << "The file " << target << "can not be written" << endl;
			sleep(1);
			continue;
		}
		if (lockFile(target))
		{
			writeMessageToFile(target, mesg);
			break;
		}
		else
		{
			cerr << "lock file failed!" << endl;
			exit(1);
		}
	}
}

int main(int ac, char* argv[])
{
	if (ac == 1) return 1;
	string targetFile = argv[1];
	const string message = "you are in file";
	const string message2 = "hhhhhhhhhh";
	writeMessageInOrder(targetFile, message, 10);
	writeMessageInOrder(targetFile, message2, 10);
	return 0;
}
