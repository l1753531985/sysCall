#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int main()
{
	struct termios info;
	int rv = -1;
	rv = tcgetattr(0, &info);
	if (rv == -1)
	{
		perror("tcgetattr");
		exit(1);
	}
	if (info.c_lflag & ECHO)
		cout << "echo is on, since its bit is 1" << endl;
	else
		cout << "echo is OFF, since its bit is 0" << endl;
	return 0;
}
