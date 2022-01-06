#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//#define SHHHH

using namespace std;

void wakeup(int signum)
{
	#ifndef SHHHH
		cout << "Alarm received from kernel" << endl;
	#endif
}

int main()
{
	cout << "about to sleep for 4 seconds" << endl;
	signal(SIGALRM, wakeup);
	alarm(4);
	pause();
	cout << "Moring so soon?" << endl;
	return 0;
}
