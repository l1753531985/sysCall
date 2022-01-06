#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "set_ticker.h"

using namespace std;

void countdown(int signum)
{
	static int num = 10;
	cout << num << endl;
	num--;
	if (num < 0)
	{
		cout << "Done" << endl;
		exit(1);
	}
}

int main()
{
	signal(SIGALRM, countdown);
	alarm(4);
	if (set_ticker(500) == -1)
		perror("set_ticker");
	else
		while(1) 
			pause();
	return 0;
}
