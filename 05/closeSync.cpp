#include <iostream>

using namespace std;

int main(int ac, char* argv[])
{
	if (ac == 1) return 0;
	string para = argv[1];
	switch (para[1])
	{
		case 'y':
		case 'Y':
			break;
		case 'n':
		case 'N':
			break;
		default:
			break;
	}
	return 0;
}
