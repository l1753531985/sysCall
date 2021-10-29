/* logout_tty(char* line)
 * marks a utmp record as logged out
 * does not blank username or remote host
 * return -1 on error, 0 on success
 */
#include <stdio.h>
#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int logout_tty(char* line)
{
	int fd;
	struct utmp rec;
	int len = sizeof(struct utmp);
	int retval = -1;

	if ((fd = open(UTMP_FILE, O_RDWR)) == -1)
		return -1;
	/* search and replace*/
	while (read(fd, &rec, len) == len)
		if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == -1)
		{
			rec.ut_type = DEAD_PROCESS;
			if (time((time_t*)(&rec.ut_tv)) != -1)
				if (write(fd, &rec, len) == len)
					retval = 0;
			break;
		}
	if (close(fd) == -1)
		retval = -1;
	return retval;
}

int main(int ac, char* av[])
{
	if (ac != 2) return -1;

	char* myline = av[--ac];
	int ret; 
	if ((ret = logout_tty(myline)) == -1)
		perror("Can not logout: ");

}
