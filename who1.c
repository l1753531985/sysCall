/* who1.c - a first version of the who program
 * open, read UTMP file, and show results 
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHOWHOST

void show_info(struct utmp*);
void show_time(long);

int main()
{
	struct utmp current_record;
	int utmpfd;
	int reclen = sizeof(current_record);

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}

	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

/* 
 * show_info()
 * displays contents of the utmp struct in human readable form 
 * *note* these sizes should not be hardwired
 */
void show_info(struct utmp* utbufp)
{
	if (utbufp->ut_type != USER_PROCESS) return;
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

