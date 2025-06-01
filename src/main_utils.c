/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:19:06 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/02 00:32:06 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_and_tokenize(char *input, t_env *env_list, t_args *arg,
	t_token **tokens)
{
	t_env		*copy;
	char		*expandable;

	copy = deep_copy_env_list(env_list);
	if (!copy)
		return (NULL);
	expandable = parse_env(input, copy, arg);
	if (!expandable)
	{
		free_env_list(copy);
		return (NULL);
	}
	*tokens = tokenize(expandable);
	if (!*tokens)
	{
		free_tokens(*tokens);
		return (NULL);
	}
	free_env_list(copy);
	return (expandable);
}

static void	execute_input(t_token *tokens, t_env *env_list, char *expandable)
{
  t_ast *ast;

  ast = parse_tokens(tokens);
  free_tokens(tokens);
  if (!ast) {
    free(expandable);
    return;
  }
  ast->heredoc_files = NULL;
  ast->heredoc_count = 0;
  ast->env_list = env_list;
  execute_tree(ast, ast);
  free_heredoc_list(ast);
  free_ast(ast);
}

void handle_input(char *input, t_env *env_list, int argc, char **argv) {
  t_token *tokens;
  t_args arg;
  char *expandable;

  arg.exit_status = 0;
  if (*input) {
    add_history(input);
    arg = (t_args){.argc = argc - 1, .argv = argv};
    expandable = expand_and_tokenize(input, env_list, &arg, &tokens);
    if (!expandable)
      return;
    if (!validation(tokens))
      return;
    execute_input(tokens, env_list, expandable);
  }
}

char *generate_prompt(void) {
  char cwd[PATH_MAX];
  char *str;

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    str = ft_strjoin("minihell:", cwd);
    return (ft_strjoin(str, "$ "));
  }
  return (ft_strdup("minishell$ "));
}
