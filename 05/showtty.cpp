#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <unordered_map>

using namespace std;

void showbaudvalue(string value)
{
	cout << value;
}

void showbaud(speed_t thespeed)
{
	cout << "the baud rate is ";
	switch (thespeed)
	{
		case B300:
			showbaudvalue("300\n");
			break;
		case B600:
			showbaudvalue("600\n");
			break;
		case B1200:
			showbaudvalue("1200\n");
			break;
		case B1800:
			showbaudvalue("1800\n");
			break;
		case B2400:
			showbaudvalue("2400\n");
			break;
		case B4800:
			showbaudvalue("4800\n");
			break;
		case B9600:
			showbaudvalue("9600\n");
			break;
		case B38400:
			showbaudvalue("38400\n");
			break;
		default:
			showbaudvalue("Fast\n");
			break;
	}
}

void show_flagset(tcflag_t thevalue, unordered_map<int, string>& thebitnames)
{
	unordered_map<int ,string>::iterator iter = thebitnames.begin();
	for (; iter != thebitnames.end(); iter++)
	{
		cout << iter->second << " is ";
		if (thevalue & iter->first)
			cout << "ON" << endl;
		else
			cout << "OFF" << endl;
	}
}

void show_some_flags(struct termios* ttyp)
{
	unordered_map<int, string> input_flags{{IGNBRK, "Ignore break condition"}, {BRKINT, "Signal interrupt on break"}, {IGNPAR, "Ignore chars with parity errors"}, {PARMRK, "Mark parity errors"}, {INPCK, "Enable input parity check"}, {ISTRIP, "Strip character"}, {INLCR, "Map NL to CR on input"}, {IGNCR, "Ignore CR"}, {ICRNL, "Map CR to NL on input"}, {IXON, "Enable start/stop output control"}, {IXANY, "enable any char to restart output"}, {IXOFF, "Enable start/stop input control"}, {0, NULL}};

	unordered_map<int, string> local_flags{{ISIG, "Enable signals"}, {ICANON, "Cannonical input(erase and kill)"}, {XCASE, "Canonical upper/lower appearance"}, {ECHO, "Enable echo"}, {ECHOE,"Echo ERASE as BS-SPACE-BS"}, {ECHOK, "Echo KILL by starting new line"}, {0, NULL}};

	show_flagset(ttyp->c_iflag, input_flags);
	show_flagset(ttyp->c_lflag, local_flags);
}

int main()
{
	struct termios ttyinfo;
	if (tcgetattr(0, &ttyinfo) == -1)
	{
		perror("cannot get params about stdin");
		exit(1);
	}
	showbaud(cfgetospeed(&ttyinfo));
	cout << "The erase character is ascii " << ttyinfo.c_cc[VERASE] << " Ctrl-" << ttyinfo.c_cc[VERASE]-1+'A' << endl; 
	cout << "The line kill character is ascii " << ttyinfo.c_cc[VKILL] << " Ctrl-" << ttyinfo.c_cc[VKILL]-1+'A' << endl;
	show_some_flags(&ttyinfo);
	return 0;
}
