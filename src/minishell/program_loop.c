/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eseferi <eseferi@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 20:51:13 by eseferi           #+#    #+#             */
/*   Updated: 2023/11/26 08:45:12 by eseferi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void start_loop(t_data *data)
{
    char *line;
    char *str;
    char *prompt;

    while (42)
    {
        reset_data(data);
        // Generate prompt with current directory
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            str = ft_strjoin("minihell:",cwd);
            prompt = ft_strjoin(str,"$ ");
        }
        else
            prompt = ft_strdup("minishell$ "); // Fallback
        
        line = readline(prompt);
        free(prompt); // Free the generated prompt
        
        if (handle_d(data, line))
            continue;
        if (ft_strlen(line) > 0)
            add_history(line);
        if (odd_quote(line, data))
            continue;
        data->input_line = trim_input(line);
        ft_strdel(&line);
        if ((special_chars(data->input_line)) || (lexical_analysis(data, data->input_line)))
            continue;
        if (init_tree(data, &data->token_list))
            continue;
        fix_tree(&data->tree);
        if (execute(data, data->tree))
            continue;
    }
}
