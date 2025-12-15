/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:29 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 17:46:31 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "hashtable.h"
#include <stdlib.h>

static char	*build_env_line(char *key, char *value)
{
	char	*line;
	char	*tmp;

	line = ft_strjoin("declare -x ", key);
	tmp = line;
	line = ft_strjoin(tmp, "=\"");
	free(tmp);
	tmp = line;
	line = ft_strjoin(tmp, value);
	free(tmp);
	tmp = line;
	line = ft_strjoin(tmp, "\"\n");
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

// TODO: Check if cmd_str is not alnum if it is should give error
char	*export_args(char **cmd_str, t_shell_env *env)
{
	size_t	i;
	char	*res;
	char	*var;
	char	*after_equal;
	size_t	num_args;

	i = 0;
	res = ft_strdup("");
	num_args = num_arguments(cmd_str);
	while (i < num_args)
	{
		var = ft_strdup(cmd_str[i + 1]);
		after_equal = ft_strchr(var, '=');
		*after_equal = '\0';
		after_equal++;
		if (!hashtable_get(env->vars, var))
			ft_lstadd_back(&env->order, ft_lstnew(ft_strdup(var)));
		hashtable_set(env->vars, var, ft_strdup(after_equal));
		free(var);
		i++;
	}
	return (res);
}

t_cmd_response	*func_built_in_export(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env, char *envp[])
{
	t_cmd_response	*res;

	(void)shell_ast;
	(void)env;
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
	else
	{
		res->output = export_args(cmd_str, env);
		res->exit_code = 0;
		return (res);
	}
	return (res);
}
