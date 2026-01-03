/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:41:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:02:25 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>

t_cmd_response	*func_built_in_env(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	t_list			*node;

	(void)shell_ast;
	(void)cmd_str;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	res->output = ft_strdup("");
	if (!res->output)
		return (free(res), NULL);
	node = env->order;
	while (node)
	{
		append_env_plain_line(&res->output, (char *)node->content, env);
		node = node->next;
	}
	res->exit_code = 0;
	return (res);
}
