/** cp1.c
 * version 1 of cp - uses read and write with tunable buffer size
 *
 * usage: cp1 src dest
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define BUFFERSIZE 4096
#define COPYMODE   0644

void oops(char*, char*);
void wirteToOtherFile(int, int);
int getFileFdInDir(char*, char*);
void writeTo(char*, char*);
void srcWriteTo(char*, char*);

int main(int ac, char* av[])
{
	if (ac != 3)
	{
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}
	srcWriteTo(av[1], av[2]);
	return 0;
}

void srcWriteTo(char* srcDirName, char* dstName)
{
	struct stat srcFileInfo; 

	if (stat(srcDirName, &srcFileInfo) == -1) oops("Cannot get stat",srcDirName);
	if (S_ISREG(srcFileInfo.st_mode)) 
		writeTo(srcDirName, dstName);
	else if (S_ISDIR(srcFileInfo.st_mode)) 
	{
		DIR* dirPtr;
		struct dirent* direntp;
		if ((dirPtr = opendir(srcDirName)) == NULL) oops("Cannot open directory: ", srcDirName);
		while ((direntp = readdir(dirPtr)) != NULL)
		{
			if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) continue;
			srcWriteTo(direntp->d_name, dstName);
		}
		if (closedir(dirPtr) == -1) oops("Cannot close directory: ", dstName);
	}
}

void writeTo(char* srcFileName, char* dstFileName)
{
	int in_fd, out_fd;
	struct stat dstFileInfo; 

	if ((in_fd = open(srcFileName, O_RDONLY)) == -1)
		oops("Cannot open", srcFileName);

	if (stat(dstFileName, &dstFileInfo) == -1) oops("Cannot get stat", dstFileName);
	out_fd = (S_ISDIR(dstFileInfo.st_mode)) ? getFileFdInDir(srcFileName, dstFileName) : open(dstFileName, O_WRONLY|O_CREAT);
	if (out_fd == -1) oops("Cannot creat", dstFileName);

	wirteToOtherFile(out_fd, in_fd);
}

int getFileFdInDir(char* filename, char* dirname)
{
	int fd = -1;
	char pathname[strlen(dirname) + strlen(filename)];
	if (stpcpy(pathname, dirname) == NULL)
		oops("Cannot copy directory name", dirname);
	strncat(pathname, filename, strlen(filename));
	printf("dstname: %s\n", pathname);
	if ((fd = open(pathname, O_RDWR|O_CREAT)) == -1)
		oops("Cannot open file to write", dirname);
	return fd;
}

void wirteToOtherFile(int fdDst, int fdSrc)
{
	int n_chars = 0;
	char buf[BUFFERSIZE];

	while ((n_chars = read(fdSrc, buf, BUFFERSIZE)) > 0)
		if (write(fdDst, buf, n_chars) != n_chars)
			oops("Write error to","");

	if (n_chars == -1)
		oops("Read error from", "");

	if (close(fdSrc) == -1 || close(fdDst) == -1)
		oops("Error closing files","");
}

void oops(char* s1, char* s2)
{
	fprintf(stderr, "Error: %s", s1);
	perror(s2);
	exit(1);
}


