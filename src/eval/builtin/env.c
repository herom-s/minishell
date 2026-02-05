/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:41:46 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 16:50:56 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>

static t_cmd_response	*init_env_res(char **output)
{
	t_cmd_response	*res;

	*output = ft_strdup("");
	if (!*output)
		return (NULL);
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
	{
		free(*output);
		return (NULL);
	}
	return (res);
}

t_cmd_response	*func_built_in_env(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	t_list			*node;
	char			*output;

	(void)shell_ast;
	(void)cmd_str;
	res = init_env_res(&output);
	if (!res)
		return (NULL);
	node = env->order;
	while (node)
	{
		if (hashtable_get(env->vars, (char *)node->content))
			append_env_plain_line(&output, (char *)node->content, env);
		node = node->next;
	}
	ft_dprintf(STDOUT_FILENO, "%s", output);
	res->exit_code = 0;
	free(output);
	return (res);
}
