#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define ASK "Do you want another transaction"
#define BEEP putchar('\a');

const int tries = 3;
const int sleeptime = 2;

using namespace std;

int get_ok_char()
{
	int c;
	while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
		;
	return c;
}

int get_response(const string& query, const int tries)
{
	cout << query << "(y/n)?";
	fflush(stdout);
	int maxTries = 0;
	while (1) 
	{
		sleep(sleeptime);	
		int input = tolower(get_ok_char());
		if (input == 'y')
			return 0;
		if (input == 'n')
			return 1;
		// test
		//cout << "maxTries: " << maxTries << endl;
		if (maxTries == 0)
			return 2;
		maxTries--;
		BEEP;
	}
}

void set_cr_noecho_mode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	ttystate.c_cc[VTIME] = 2;
	tcsetattr(0, TCSANOW, &ttystate);
}

int tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;
	if (how == 0)
	{
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if (stored)
	{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

void ctrl_c_handle(int signum)
{
	tty_mode(1);
	exit(1);
}

int main()
{
	tty_mode(0);
	set_cr_noecho_mode();
	signal(SIGINT, ctrl_c_handle);
	signal(SIGQUIT, SIG_IGN);
	int response = get_response(ASK, tries);
	tty_mode(1);
	return response;
}
