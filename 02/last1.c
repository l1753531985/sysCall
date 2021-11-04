#include <iostream>
#include <stdio.h>
#include <utmp.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <unordered_map>

using namespace std;

void showtime(long);
void show_login_info(struct utmp*);

int main()
{
	struct utmp utbuf;
	int wtmpfd;

       	if ((wtmpfd = open(WTMP_FILE, O_RDONLY)) == -1)
	{
		perror(WTMP_FILE);
		exit(-1);
	}

	unordered_map<string, string> logoutInfo;

	while (read(wtmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) 
		if (utbuf.ut_type != USER_PROCESS)
		{
			long timeval = (long)(utbuf.ut_time);
			logoutInfo[utbuf.ut_line] = ctime(&timeval);
		}

	if (lseek(wtmpfd, 0, SEEK_SET) == -1)
		perror("Cannot Set:");

	while (read(wtmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) 
		if (utbuf.ut_type == USER_PROCESS)
		{
			show_login_info(&utbuf);
			cout << logoutInfo[utbuf.ut_line] << endl; 
		}

	close(wtmpfd);
	return 0;
}

void show_login_info(struct utmp* utbufp)
{
	printf("%-8.8s", utbufp->ut_user);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf("\t");
	printf(utbufp->ut_host);
	printf("\t");
	showtime(utbufp->ut_time);
	printf(" - xxx\n");
}

void showtime(long timeval)
{
	char* cp = ctime(&timeval);
	printf("%16.16s", cp);
}


