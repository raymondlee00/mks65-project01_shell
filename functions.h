int num_args(char *line);
char **parse(char *line, char *delim);
char *removeSpaces(char *line);
void remove_newLine(char *line);
void redirectOut(char *cmd);
void redirectIn(char *cmd);
void redirectInNOut(char *cmd);
void pipeThis(char *cmd);
