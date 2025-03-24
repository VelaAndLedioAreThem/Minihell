/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:39:34 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/20 13:45:13 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	clear_data(t_env **data, char **envp)
{
	*data = malloc(sizeof(t_env));
	if (!*data)
		exit(EXIT_FAILURE);
	(*data)->env_list = NULL;
	(*data)->heredoc_file = NULL;
	(*data)->sorted_env_list = NULL;
	(*data)->token_list = NULL;
	(*data)->input_line = NULL;
	(*data)->tree = NULL;
	(*data)->root_directory = NULL;
	(*data)->exit_status = 0;
	(*data)->input_minishell = "minishell>> ";
	(*data)->pid = getpid();
	(*data)->curr_dir = getcwd(NULL, 0);
	fill_env(envp, *data);
	incr_shell_lvl(*data);
}
void	reset_data(t_data *data)
{
	data->single_quote = 0;
	data->double_quote = 0;
	data->forked = 0;
	if (data->root_directory && *data->root_directory)
		free_2darray(data->root_directory);
	if (data->heredoc_file)
	{
		free_heredoc_files(data->heredoc_file);
		data->heredoc_file = NULL;
	}
	data->root_directory = get_root_directory();
	if (data->input_line && ft_strlen(data->input_line) > 0)
		ft_strdel(&data->input_line);
	if (data->tree)
	{
		free_tree(&data->tree);
		data->tree = NULL;
	}
	if (data->token_list)
		free_tokens(&data->token_list, free);
}