/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:11:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/13 15:25:33 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_token			*tokens;
	t_commands		*cmd;

	while (true)
	{
		input = readline("minishell$: ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			tokens = tokenize(input);
		}
	}
}

