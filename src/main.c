/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:11:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/14 01:10:09 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char			*input;
	t_token			*tokens;

	while (true)
	{
		input = readline("minishell$: ");
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			exit(0);
		}
		if (*input)
		{
			add_history(input);
			tokens = tokenize(input);
		}
	}
}
