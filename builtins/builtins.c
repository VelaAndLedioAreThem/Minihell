/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:23:13 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 17:50:38 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int	my_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char **parse_command(char *input)
{
    int max_tokens = 64;
    char **tokens = malloc(max_tokens * sizeof(char *));
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        tokens[i++] = strdup(token);
        if (i >= max_tokens - 1)
            break;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;

    // Handle $? substitution
    int j = 0;
    while (tokens[j]) {
        if (my_strcmp(tokens[j], "$?") == 0) {
            free(tokens[j]);
            char exit_str[12];
            snprintf(exit_str, sizeof(exit_str), "%d", g_shell.last_exit_status);
            tokens[j] = strdup(exit_str);
        }
        j++;
    }

    return tokens;
}

void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i]) {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}


void	execute_command(char **args)
{
	char	*path_env;
	char	*paths_copy;
	char	*token;
	int		found;
	int		len;
	char	*full_path;

	if (strchr(args[0], '/') != NULL)
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		path_env = getenv("PATH");
		if (!path_env)
		{
			perror("getenv");
			exit(EXIT_FAILURE);
		}
		paths_copy = strdup(path_env);
		if (!paths_copy)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		token = strtok(paths_copy, ":");
		found = 0;
		while (token != NULL)
		{
			len = strlen(token) + 1 + strlen(args[0]) + 1;
			full_path = malloc(len);
			if (!full_path)
			{
				perror("malloc");
				free(paths_copy);
				exit(EXIT_FAILURE);
			}
			snprintf(full_path, len, "%s/%s", token, args[0]);
			if (access(full_path, X_OK) == 0)
			{
				found = 1;
				if (execve(full_path, args, environ) == -1)
				{
					perror("execve");
					free(full_path);
					free(paths_copy);
					exit(EXIT_FAILURE);
				}
			}
			free(full_path);
			token = strtok(NULL, ":");
		}
		free(paths_copy);
		if (!found)
		{
			write(2, "Command not found\n", 18);
			exit(EXIT_FAILURE);
		}
	}
}

void	execute_pipeline(char **commands[], int num_commands)
{
	int		pipefd[2];
	pid_t	pid;
	int		in_fd;

	in_fd = STDIN_FILENO;
	for (int i = 0; i < num_commands; i++)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			dup2(in_fd, STDIN_FILENO);
			if (i != num_commands - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			handle_redirections(commands[i]);
			if (!handle_builtin(commands[i]))
				execute_command(commands[i]);
			exit(EXIT_SUCCESS);
		}
		close(pipefd[1]);
		in_fd = pipefd[0];
	}
	while (wait(NULL) > 0)
		;
}
