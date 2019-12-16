# OOGA BOOGA SHELL

by Tyler Huang and Raymond Lee

## Features:
- Can execute commands.
- Can execute multiple commands in one line.
- Redirects and double-redirects with "<" and ">".
- Supports piping
- type ./test.txt to run test commands

## Bugs:

- Extra spaces between commands break the command.
- Segmentation fault after running ps ; echo HELLO (seg fault appears on mac after echo HELLO)
- Segmentation fault with double redirects

## Files and function headers

### main.c

    /*========== void printInit(void) ==========
	Inputs:  none
	Returns: none

	Prints [User:CURRENTDIR] in front of each line in the shell.
	============================================*/

	/*========== int main() ==========
	Inputs:  none
	Returns: EXIT_SUCCESS

  	Runs the shell: uses other functions to read, parse, and execute each line.
	=================================*/

### functions.c

	/*========== int num_args(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: int

	Returns the number of arguments in a command, separated by " ".
	================================================*/

	/*========== char **parse(char *line, char *delimiter) ==========
	Inputs:  char *line (line of input), char *delimiter (delimiter string)
	Returns: char **

	Returns the parsed array of the line split by delimiter.
	=================================================================*/

	/*========== void remove_newLine(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: none

	Removes the newline character '\n' in the last character of a string.
	=======================================================*/

	/*========== char *removeSpaces(char *line) ==========
	Inputs:  char *line (line of input)
	Returns: char *

	Removes the newline character '\n' in the last character of a string.
	=====================================================*/

	/*========== void redirectOut(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Redirects output to another file.
	===================================================*/

	/*========== void redirectIn(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Redirects input from file to command.
	=================================================*/

	/*========== void redirectInNOut(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Supports redirection between file and command both ways in one command.
	======================================================*/

	/*========== void pipeThis(char *cmd) ==========
	Inputs:  char *cmd (command)
	Returns: none

	Supports piping between commands using "|".
	======================================================*/
