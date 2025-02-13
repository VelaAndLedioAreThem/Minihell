/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:23:16 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 17:49:40 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int	handle_env_commands(char **args)
{
	char	*equal_sign;
	int		name_len;
	char	*varname;
	char	*val;
	int		ret;

	if (!my_strcmp(args[0], "export"))
	{
		printf("Running export\n");
		if (!args[1])
		{
			for (char **env = environ; *env; env++)
				printf("%s\n", *env);
		}
		else
		{
			for (int i = 1; args[i]; i++)
			{
				equal_sign = strchr(args[i], '=');
				if (equal_sign)
				{
					name_len = equal_sign - args[i];
					varname = malloc(name_len + 1);
					if (!varname)
					{
						perror("malloc");
						exit(EXIT_FAILURE);
					}
					strncpy(varname, args[i], name_len);
					varname[name_len] = '\0';
					ret = my_setenv(varname, equal_sign + 1, 1);
					if (ret == 0)
						printf("Exported: %s\n", args[i]);
					else
						printf("Failed to export: %s\n", args[i]);
					free(varname);
				}
				else
				{
					val = getenv(args[i]);
					if (!val)
						printf("No value to export for: %s\n", args[i]);
				}
			}
		}
		return (1);
	}
	else if (!my_strcmp(args[0], "unset"))
	{
		if (!args[1])
			fprintf(stderr, "unset: not enough arguments\n");
		else
		{
			for (int i = 1; args[i]; i++)
			{
				ret = my_unsetenv(args[i]);
				if (ret != 0)
					printf("Failed to unset: %s\n", args[i]);
			}
		}
		return (1);
	}
	else if (!my_strcmp(args[0], "env"))
	{
		for (char **env = environ; *env; env++)
			printf("%s\n", *env);
		return (1);
	}
	return (0);
}

// Handles directory-related commands: cd, pwd
int	handle_directory_commands(char **args)
{
	char	*home;
		char cwd[1024];

	if (!my_strcmp(args[0], "cd"))
	{
		if (!args[1])
		{
			home = getenv("HOME");
			if (!home)
            {
                fprintf(stderr, "cd: HOME not set\n");
                g_shell.last_exit_status = 0;
                
            }
			else if (chdir(home) != 0)
			{
				printf("couldnt navigate to home :(");
                g_shell.last_exit_status = 1;
			}
		}
		else if (chdir(args[1]) != 0)
			perror("cd");
		return (1);
	}
	else if (!my_strcmp(args[0], "pwd"))
	{
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
		else
			perror("pwd");
		return (1);
	}
	return (0);
}

// Handles remaining commands: echo, exit
int	handle_other_commands(char **args)
{
	int	newline = 1, i;

	if (!my_strcmp(args[0], "echo"))
	{
		newline = 1, i = 1;
		if (args[1] && !my_strcmp(args[1], "-n"))
		{
			newline = 0;
			i++;
		}
		for (; args[i]; i++)
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
		}
		if (newline)
			printf("\n");
		return (1);
	}
	else if (!my_strcmp(args[0], "exit")) {
        int status = g_shell.last_exit_status;
        if (args[1]) status = atoi(args[1]);
        exit(status);
    }
	return (0);
}
int	handle_builtin(char **args)
{
	if (handle_env_commands(args))
		return (1);
	if (handle_directory_commands(args))
		return (1);
	if (handle_other_commands(args))
		return (1);
	return (0);
}