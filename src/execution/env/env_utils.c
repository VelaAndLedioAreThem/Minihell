/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 12:34:56 by user              #+#    #+#             */
/*   Updated: 2025/04/10 21:18:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

void	print_env_list(t_env *env_list, int fd_out)
{
	while (env_list)
	{
		ft_putstr_fd(env_list->key, fd_out);
		ft_putstr_fd("=", fd_out);
		if (env_list->value)
			ft_putendl_fd(env_list->value, fd_out);
		else
			ft_putendl_fd("", fd_out);
		env_list = env_list->next;
	}
}

static int	count_env_nodes(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	**env(t_env **lst)
{
	t_env	*tmp;
	char	**envp;
	int		i;
	int		count;

	count = count_env_nodes(*lst);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	tmp = *lst;
	i = 0;
	while (tmp)
	{
		envp[i++] = ft_strjoin3(tmp->key, "=", tmp->value);
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

static void	create_new_shlvl(t_env *data, int shlvl)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup("SHLVL");
	new_env->value = ft_itoa(shlvl);
	new_env->next = data;
	data = new_env;
}
