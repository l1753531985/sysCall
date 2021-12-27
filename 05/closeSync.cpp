#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SETSYNC(m) m | O_SYNC
#define CALCELSYNC(m) m & (~O_SYNC)

using namespace std;

void setFileStatusFlag(bool isSet)
{
	// set flag
	const string bigFile = "/root/sysCall/05/bigTest.txt";
	int fd = -1;
	if ((fd = open(bigFile.c_str(), O_RDONLY)) == -1)
	{
		perror(bigFile.c_str());
		exit(1);
	}
	
	int s = fcntl(fd, F_GETFL);
	s = (isSet) ? SETSYNC(s) : CALCELSYNC(s);	
	if (fcntl(fd, F_SETFL, s) == -1)
	{
		perror("setting SYNC");
		exit(1);
	}

	// write 
	const int size = 16;
	char* tmp = new char[size];
	int realSize = -1;
	while ((realSize = read(fd, tmp, size)) > 0)
		cout << tmp << endl;
		//if (write(fd, tmp, realSize) == -1)

	delete[] tmp;

	// close file 
	if (close(fd) == -1)
	{
		cerr << "close " << bigFile << "failed" << endl;
		exit(1);
	}	
}

int main(int ac, char* argv[])
{
	if (ac == 1) return 0;
	string para = argv[1];
	switch (para[0])
	{
		case 'y':
		case 'Y':
			setFileStatusFlag(true);
			break;
		case 'n':
		case 'N':
			setFileStatusFlag(false);
			break;
		default:
			break;
	}
	return 0;
}
