#include <stdio.h>
#include <string>
#include <unistd.h>
#include <curses.h>

using namespace std;

int main()
{

	static int leftedge = 10; 
	static int rightedge = 30; 
	static int row = 10; 

	string message = "Hello";
	string blank = "        ";
	int dir = 1;
	int pos = leftedge;

	initscr();
	clear();
	while (1) 
	{
		move(row, pos);
		addstr(message.c_str());
		move(LINES-1, COLS-1);
		refresh();
		sleep(1);
		move(row, pos);
		addstr(blank.c_str());
		pos += dir;
		if (pos >= rightedge)
			dir = -1;
		if (pos <= leftedge)
			dir = 1;
	}
	endwin();
	return 0;
}
