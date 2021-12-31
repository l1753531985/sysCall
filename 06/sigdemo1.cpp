#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

using namespace std;

void f(int signum)
{
	cout << "OUCH!" << endl;
}

int main()
{
	//signal(SIGINT, f);
	signal(SIGINT, SIG_IGN);
	for (int i = 0; i < 5; i++)
	{
		cout << "hello" << endl;
		sleep(1);
	}
	return 0;
}
