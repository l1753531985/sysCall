#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main()
{
	struct termios termios_p;
	if (tcgetattr(0, &termios_p) == -1)
	{
		perror("tty");
		exit(1);
	}
	string res = (termios_p.c_oflag & OLCUC) ? "ON" : "OFF";
	cout << "OLCUC function is " << res << " currently" << endl;
	return 0;
}
