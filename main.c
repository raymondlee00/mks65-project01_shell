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

/*========== void printInit(void) ==========
	Inputs:  none
	Returns: none

	Prints [User:CURRENTDIR] in front of each line in the shell.
============================================*/
void printInit(void)
{
	char cwd[512];
	char *username = getenv("USER");
	getcwd(cwd, sizeof(cwd));
	printf("%s:%s$ ", username, cwd);
}

/*========== int main() ==========
	Inputs:  none
	Returns: EXIT_SUCCESS

  	Runs the shell: uses other functions to read, parse, and execute each line.
=================================*/
int main()
{
	char line[100];
	char **args;
	char **cmds;
	int t;
	do
	{
		printInit(); //printing current directory
		fgets(line, 120, stdin); //getting user input
		remove_newLine(line);	//removing the \n from line
		cmds = parse(line, ";");
		int i;
		for (i = 0; cmds[i] != NULL; i++)
		{ //executing each command with loop
			cmds[i] = removeSpaces(cmds[i]);
			if (strchr(cmds[i], '<') != NULL && strchr(cmds[i], '>') != NULL)
			{ //if <>
				redirectInNOut(cmds[i]);
			}
			else if (strchr(cmds[i], '<') != NULL)
			{ //if <
				redirectIn(cmds[i]);
			}
			else if (strchr(cmds[i], '>') != NULL)
			{ //if >
				redirectOut(cmds[i]);
			}
			else if (strchr(cmds[i], '|') != NULL)
			{ //if |
				pipeThis(cmds[i]);
			}
			else
			{
				args = parse(cmds[i], " ");
				if (!strcmp(args[0], "exit"))
				{ //if exit
					//free memory
					free(cmds[0]);
					free(cmds);
					free(args[0]);
					free(args);
					return(EXIT_SUCCESS);
				}
				if (!strcmp(args[0], "cd"))
				{					//if cd
					chdir(args[1]); //change directory
									//free memory
					free(args[0]);
					free(args);
				}
				else
				{
					if (!fork())
					{
						if (execvp(args[0], args) == -1)
							exit(-1); //error
					}
					//free memory
					free(args[0]);
					free(args);
				}
			}
			wait(&t);
		}
		//free memory
		if (i > 0)
		{
			free(cmds[0]);
		}
		free(cmds);
	} while(1);

	return(EXIT_SUCCESS);
}
