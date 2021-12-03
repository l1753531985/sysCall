#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

void creatDir(const string& dirname)
{
	if (mkdir(dirname.c_str(), 0644) == -1)
		cerr << "Cannot create dirtory: " << dirname << endl;
}

void changDir(const string& dirname)
{
	if (chdir(dirname.c_str()) == -1)
		cerr << "Cannot change work directory: " << dirname << endl;
}

void do_creat(const string& dirname)
{
	if (dirname[0] == '/')
		chdir("/");
	chdir(dirname.c_str());

	const string firstMenu = "demodir", secondMenu1 = "a", secondMenu2 = "c", thirdMenu1 = "d1", thirdMenu2 = "d2";

	creatDir(firstMenu);
	changDir(firstMenu);
	creatDir(secondMenu1);
	creatDir(secondMenu2);
	changDir(secondMenu2);
	creatDir(thirdMenu1);
	creatDir(thirdMenu2);
}

int main(int ac, char* argv[])
{
	if (ac == 1)
		do_creat(".");
	else
	{
		do_creat(argv[1]);
	}
	return 0;
}
