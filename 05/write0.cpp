#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

using namespace std;

int main(int ac, char* argv[])
{
	if (ac != 2) 
	{
		cerr << "usage: write0 ttyname" << endl;
		return -1;
	}
	int fd = open(argv[1], O_WRONLY);
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
