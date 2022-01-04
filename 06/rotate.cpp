#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;
	if (how == 0)
	{
		if (tcgetattr(0, &original_mode) == -1)
			exit(1);
		if ((original_flags = fcntl(0, F_GETFL)) == -1)
			exit(2);
		stored = 1;
	}
	else if (stored == 1)
	{
		if (tcsetattr(0, TCSANOW, &original_mode) == -1)
			exit(-1);
		if (fcntl(0, F_SETFL, original_flags) == -1)
			exit(-2);
	}
}

void close_cr_noecho_mode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &ttystate);
}

void ctrl_c_handler(int signum)
{
	tty_mode(1);
	exit(-3);
}

int main()
{
	tty_mode(0);	
	close_cr_noecho_mode();
	int c = -1;
	signal(SIGINT, ctrl_c_handler);
	while ((c = getchar()) != EOF)
	{
		if (c == 'Q') 
			break;
		if (c == 'z')
			c = 'a';
		else if (islower(c))
			c++;
		//cout << c << endl;
		putchar(c);
	}
	tty_mode(1);	
	return 0;
}
