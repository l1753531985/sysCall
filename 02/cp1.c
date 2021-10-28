/** cp1.c
 * version 1 of cp - uses read and write with tunable buffer size
 *
 * usage: cp1 src dest
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE   0644

void oops(char*, char*);

int main(int ac, char* av[])
{
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];

	if (ac != 3)
	{
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}

	if ((in_fd = open(av[1], O_RDONLY)) == -1)
		oops("Cannot open", av[1]);

	if ((out_fd = open(av[2], O_WRONLY|O_CREAT)) == -1)
		oops("Cannot creat", av[2]);

	while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, n_chars) != n_chars)
			oops("Write error to", av[2]);
	if (n_chars == -1)
		oops("Read error from", av[1]);

	if (close(in_fd) == -1 || close(out_fd) == -1)
		oops("Error closing files","");

	return 0;
}

void oops(char* s1, char* s2)
{
	fprintf(stderr, "Error: %s", s1);
	perror(s2);
	exit(1);
}


