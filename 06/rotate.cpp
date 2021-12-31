#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
	int c = -1;
	while ((c = getchar()) != EOF)
	{
		if (c == 'z')
			c = 'a';
		else if (islower(c))
			c++;
		putchar(c);
	}
	return 0;
}
