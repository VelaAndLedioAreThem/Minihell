/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:34:51 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 16:42:59 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	flush_prev(t_args *p, char *in)
{
	char	*seg;
	char	*old;

	if (p->i <= p->start)
		return (1);
	seg = ft_substr(in, p->start, p->i - p->start);
	if (!seg)
		return (0);
	old = p->result;
	p->result = ft_strjoin(p->result, seg);
	free(old);
	free(seg);
	return (1);
}

static void	append_literal(t_args *p, const char *lit)
{
	char	*old;

	old = p->result;
	p->result = ft_strjoin(p->result, (char *)lit);
	free(old);
}

static int	backslash_block(t_args *p, char *in)
{
	int	j;
	int	bs;

	j = p->i;
	bs = 0;
	while (in[j] == '\\')
	{
		++j;
		++bs;
	}
	if (!bs || in[j] != '$')
		return (0);
	if (!flush_prev(p, in) || !append_backslashes(p, bs / 2))
		return (-1);
	if (bs % 2)
		append_literal(p, "$");
	p->i = j + (bs % 2);
	p->start = p->i;
	return (1);
}

static int	expand_if_needed(t_args *p, t_env *env, char *in)
{
	int	old;

	if (in[p->i] != '$' || !in[p->i + 1]
		|| ft_strchr(" '\"", in[p->i + 1]))
		return (0);
	if (is_after_heredoc(in, p->i))
		return (++p->i, 1);
	old = p->i;
	p->result = handle_env_part(p, &p->i, env);
	if (p->i == old)
		++p->i;
	p->start = p->i;
	return (1);
}

void	process_env_var(t_args *p, t_env *env, char *in)
{
	int	ret;

	ret = backslash_block(p, in);
	if (ret)
		return ;
	if (quotes(in, p->i, p))
		return ((void)(++p->i));
	if (expand_if_needed(p, env, in))
		return ;
	++p->i;
}
