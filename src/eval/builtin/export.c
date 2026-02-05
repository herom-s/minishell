/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:42:29 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 16:02:22 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

static int	count_env_vars(t_list *order)
{
	int		count;
	t_list	*node;

	count = 0;
	node = order;
	while (node)
	{
		if (ft_strncmp((char *)node->content, "_", 2) != 0)
			count++;
		node = node->next;
	}
	return (count);
}

static char	**collect_keys(t_list *order, int count)
{
	char	**keys;
	t_list	*node;
	int		i;

	keys = ft_calloc(count + 1, sizeof(char *));
	if (!keys)
		return (NULL);
	node = order;
	i = 0;
	while (node)
	{
		if (ft_strncmp((char *)node->content, "_", 2) != 0)
			keys[i++] = (char *)node->content;
		node = node->next;
	}
	return (keys);
}

static void	sort_keys(char **keys, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(keys[j], keys[j + 1]) > 0)
			{
				tmp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	*export_no_args(t_shell_env *env)
{
	char	*res;
	char	**keys;
	int		count;
	int		i;

	res = ft_strdup("");
	count = count_env_vars(env->order);
	if (count == 0)
		return (res);
	keys = collect_keys(env->order, count);
	if (!keys)
		return (res);
	sort_keys(keys, count);
	i = 0;
	while (i < count)
	{
		append_env_line(&res, keys[i], env);
		i++;
	}
	free(keys);
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

static void	print_and_free(char *out, char *err)
{
	if (out)
		ft_dprintf(STDOUT_FILENO, "%s", out);
	if (err)
		ft_dprintf(STDERR_FILENO, "%s", err);
	free(out);
	free(err);
}

t_cmd_response	*func_built_in_export(t_ast *ast, char **cmd, t_shell_env *env)
{
	char			*out;
	char			*err;
	t_cmd_response	*res;

	(void)ast;
	out = NULL;
	err = NULL;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (num_arguments(cmd) == 0)
	{
		out = export_no_args(env);
		res->exit_code = 0;
	}
	else
		res->exit_code = export_args(cmd, env, &out, &err);
	print_and_free(out, err);
	return (res);
}
