#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define INPUTLEN 100

using namespace std;

void inthandler(int s)
{
	cout << "Received signal " << s << " .. waiting" << endl;
	sleep(2);
	cout << "Leaving inthandler" << endl;
}

void quithandler(int s)
{
	cout << "Received signal " << s << " .. waiting" << endl;
	sleep(3);
	cout << "Leaving quithandler" << endl;
}

int main(int ac, char* av[])
{
	char input[INPUTLEN];
	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);
	int nchars = -1;
	do {
		cout << "\nType a message" << endl;
		nchars = read(0, input, (INPUTLEN-1));
		if (nchars == -1)
			perror("read returned an error");
		else
		{
			input[nchars] = '\0';
			cout << "You typed: " << input << endl;
		}
	} while (strncmp(input, "quit", 4) != 0);
	return 0;
}
