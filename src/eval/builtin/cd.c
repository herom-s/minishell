/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:41:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 15:14:37 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stddef.h>
#include <unistd.h>

static size_t	num_arguments(t_ast *shell_ast)
{
	size_t	i;
	t_ast	*node;

	i = 0;
	node = shell_ast->s_simple_cmd.cmd_suffix;
	while (node)
	{
		i++;
		node = node->s_cmd_suffix.cmd_suffix;
	}
	return (i);
}

t_cmd_response	*func_built_in_cd(t_ast *shell_ast, char **cmd_str,
		char *envp[])
{
	t_cmd_response	*res;
	size_t			num_args;

	(void)shell_ast;
	(void)cmd_str;
	(void)envp;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	num_args = num_arguments(shell_ast);
	if (num_args == 1)
	{
		chdir(shell_ast->s_simple_cmd.cmd_suffix->s_cmd_suffix.word);
		res->curr_dir = get_curdir();
		res->output = ft_strdup("");
		res->exit_code = 0;
	}
	else if (num_args > 1)
	{
		res->erro_msg = ft_strdup("too many arguments\n");
		res->exit_code = 1;
		return (res);
	}
	return (res);
}
