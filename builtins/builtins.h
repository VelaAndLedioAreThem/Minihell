#ifndef BUILTINS_H
#define BUILTINS_H

#include "../shell.h"

void handle_redirections(char **args) ;
int handle_builtin(char **args);

#endif 
