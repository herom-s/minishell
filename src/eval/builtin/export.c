/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:29 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 16:52:17 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>
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
		t_shell_env *env)
{
	char			*output;
	char			*error_msg;
	t_cmd_response	*res;

	(void)shell_ast;
	output = NULL;
	error_msg = NULL;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
	{
		free(output);
		free(error_msg);
		return (NULL);
	}
	if (num_arguments(cmd_str) == 0)
	{
		output = export_no_args(env);
		ft_dprintf(STDOUT_FILENO, "%s", output);
		res->exit_code = 0;
		free(output);
		return (res);
	}
	res->exit_code = export_args(cmd_str, env, &output, &error_msg);
	ft_dprintf(STDOUT_FILENO, "%s", output);
	free(output);
	ft_dprintf(STDERR_FILENO, "%s", error_msg);
	free(error_msg);
	return (res);
}
