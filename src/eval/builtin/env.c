/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:41:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 14:22:58 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "hashtable.h"
#include "libft.h"
#include <unistd.h>

static char	*build_env_line(char *key, char *value)
{
	char	*line;
	char	*tmp;

	line = ft_strjoin(key, "=");
	tmp = line;
	line = ft_strjoin(tmp, value);
	free(tmp);
	tmp = line;
	line = ft_strjoin(tmp, "\n");
	free(tmp);
	return (line);
}

static void	append_env_line(char **output, char *key, t_shell_env *env)
{
	char	*value;
	char	*line;
	char	*tmp;

	value = hashtable_get(env->vars, key);
	line = build_env_line(key, value);
	tmp = *output;
	*output = ft_strjoin(tmp, line);
	free(tmp);
	free(line);
}

t_cmd_response	*func_built_in_env(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env, char *envp[])
{
	t_cmd_response	*res;
	t_list			*node;

	(void)shell_ast;
	(void)cmd_str;
	(void)envp;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	res->output = ft_strdup("");
	if (!res->output)
		return (free(res), NULL);
	node = env->order;
	while (node)
	{
		append_env_line(&res->output, (char *)node->content, env);
		node = node->next;
	}
	res->exit_code = 0;
	return (res);
}
