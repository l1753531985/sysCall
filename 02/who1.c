/* who3.c - who with buffered reads 
 * - supresses empty records
 * - formats time nicely
 * - buffers input (using utmplib)
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define SHOWHOST

void show_info(struct utmp*);
void show_time(long);

int main(int ac, char* argv[])
{
	struct utmp* utbufp;
	struct utmp* utmp_next();
	int utmp_open(char*);
	void utmp_close();

	char* para;
	if (ac == 2) 
		para = argv[1];
	else
		para = "user";

	if (utmp_open(UTMP_FILE) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}

	while ((utbufp = utmp_next()) != ((struct utmp*)NULL))
	{
		if (strcmp(para,"user") == 0)
		{
			if (utbufp->ut_type == USER_PROCESS) 
				show_info(utbufp);
		}
		else if (strcmp(para,"exit") == 0)
		{
			if (utbufp->ut_type == DEAD_PROCESS) 
				show_info(utbufp);
		}
		else
			show_info(utbufp);
	}
	utmp_close();
	return 0;
}

/* 
 * show_info()
 * displays contents of the utmp struct in human readable form 
 * *note* these sizes should not be hardwired
 */
void show_info(struct utmp* utbufp)
{
	printf("%-8.8s", utbufp->ut_user);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	//printf("%10ld", utbufp->ut_tv);
	show_time(utbufp->ut_tv.tv_sec);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);
#endif
	printf("\n");
}

void show_time(long timeval)
{
	char* cp = ctime(&timeval);
	printf("%12.12s",cp+4);
}

