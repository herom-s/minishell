/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:29 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/16 14:26:50 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>

char	*export_no_args(t_shell_env *env)
{
	char	*res;
	t_list	*node;

	res = ft_strdup("");
	node = env->order;
	while (node)
	{
		append_env_line(&res, (char *)node->content, env);
		node = node->next;
	}
	return (res);
}

int	export_args(char **cmd_str, t_shell_env *env, char **output, char **error)
{
	size_t	i;
	int		has_error;
	size_t	num_args;

	i = 0;
	has_error = 0;
	*output = ft_strdup("");
	num_args = num_arguments(cmd_str);
	while (i < num_args)
	{
		if (process_export_arg(cmd_str[i + 1], env, error))
			has_error = 1;
		i++;
	}
	return (has_error);
}

t_cmd_response	*func_built_in_export(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env, char *envp[])
{
	t_cmd_response	*res;

	(void)shell_ast;
	(void)envp;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (num_arguments(cmd_str) == 0)
	{
		res->output = export_no_args(env);
		res->exit_code = 0;
		return (res);
	}
	res->exit_code = export_args(cmd_str, env, &res->output, &res->erro_msg);
	return (res);
}
