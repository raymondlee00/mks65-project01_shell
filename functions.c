#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "functions.h"

/*========== int num_args(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: int

	Returns the number of arguments in a command, separated by " ".
================================================*/
int num_args(char *line)
{
	if (line || line[0])
	{ // empty command
		return 1;
	}
	int numArgs = 1;
	for (int j = 0; j < sizeof(line); j++)
	{
		if (line[j] == ' ')
		{
			numArgs++;
		}
	}
	return numArgs;
}

/*========== char **parse(char *line, char *delimiter) ==========
	Inputs:  char *line (line of input), char *delimiter (delimiter string)
	Returns: char **

	Returns the parsed array of the line split by delimiter.
=================================================================*/
char **parse(char *line, char *delimiter)
{
	char *args = calloc(420, 1);
	strcpy(args, line);
	char **parsed = malloc(num_args(line) * sizeof(char *));
	int i;

	for (i = 0; args != NULL; i++)
		parsed[i] = strsep(&args, delimiter);

	parsed[i] = NULL;
	return parsed;
}

/*========== void remove_newLine(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: none

	Removes the newline character '\n' in the last character of a string.
=======================================================*/
void remove_newLine(char *line)
{
	int len;
	len = strlen(line);
	if (line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
	}
}

/*========== char *removeSpaces(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: char *

	Removes the newline character '\n' in the last character of a string.
=====================================================*/
char *removeSpaces(char *line)
{
	int len;
	len = strlen(line);
	if (line[0] == ' ')
	{
		line++;
	}
	if (line[len - 1] == ' ')
	{
		line[len - 1] = '\0';
	}
	return line;
}

/*========== void redirectOut(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Redirects output to another file.
===================================================*/
void redirectOut(char *cmd)
{
	char **cmds = parse(cmd, ">");
	char **args = parse(removeSpaces(cmds[0]), " ");
	char *file = removeSpaces(cmds[1]);
	int fd = open(file, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0755);
	if (fd == -1)
	{
		fd = open(file, O_WRONLY | O_TRUNC);
	}
	if (!fork())
	{
		dup2(fd, STDOUT_FILENO);
		if (execvp(args[0], args) == -1)
			exit(-1); //error
	}
	free(cmds[0]);
	free(cmds);
	free(args[0]);
	free(args);
	close(fd);
}

/*========== void redirectIn(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Redirects input from file to command.
=================================================*/
void redirectIn(char *cmd)
{
	char **cmds = parse(cmd, "<");
	char **args = parse(removeSpaces(cmds[0]), " ");
	char *file = removeSpaces(cmds[1]);
	int fd = open(file, O_RDONLY);

	if (!fork())
	{
		dup2(fd, STDIN_FILENO);
		if (execvp(args[0], args) == -1)
			exit(-1); //error
	}

	free(cmds[0]);
	free(cmds);
	free(args[0]);
	free(args);
	close(fd);
}

/*========== void redirectInNOut(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Supports redirection between file and command both ways in one command.
======================================================*/
void redirectInNOut(char *cmd)
{
	char **cmds = parse(cmd, "<>");
	char **args = parse(removeSpaces(cmds[0]), " ");
	char *file = removeSpaces(cmds[1]);
	char *file2 = removeSpaces(cmds[2]);
	int fd1 = open(file, O_RDONLY);
	int fd2 = open(file2, O_WRONLY);

	if (!fork())
	{
		dup2(fd1, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		if (execvp(args[0], args) == -1)
			exit(-1); //error
	}

	free(cmds[0]);
	free(cmds);
	free(args[0]);
	free(args);
	close(fd1);
	close(fd2);
}

/*========== void pipeThis(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Supports piping between commands using "|".
======================================================*/
void pipeThis(char *cmd)
{
	char **cmds = parse(cmd, "|");
	char **cmd1 = parse(removeSpaces(cmds[0]), " ");
	char **cmd2 = parse(removeSpaces(cmds[1]), " ");
	int p[2];
	pipe(p);

	if (!fork())
	{
		dup2(p[1], STDOUT_FILENO);
		if (execvp(cmd1[0], cmd1) == -1)
			exit(-1); //error
	}

	int t;
	wait(&t);

	if (!fork())
	{
		dup2(p[0], STDIN_FILENO);
		close(p[1]);
		if (execvp(cmd2[0], cmd2) == -1)
			exit(-1); //error
	}

	free(cmds[0]);
	free(cmds);
	free(cmd1[0]);
	free(cmd1);
	free(cmd2[0]);
	free(cmd2);
	close(p[0]);
	close(p[1]);
	wait(&t);
}
