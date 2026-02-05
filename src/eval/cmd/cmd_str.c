/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:47:15 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "expand.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

static void	fill_cmd_suffix(char **cmd_str, t_ast *cmd_suffix, int *i)
{
	t_ast	*node;

	node = cmd_suffix;
	while (node)
	{
		if (node->u_ast.s_cmd_suffix.word)
			cmd_str[(*i)++] = ft_strdup(node->u_ast.s_cmd_suffix.word);
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
}

static char	**build_suffix_only(t_ast *cmd_suffix)
{
	char	**cmd_str;
	int		count;
	int		i;

	count = count_cmd_suffix(cmd_suffix);
	if (count == 0)
		return (NULL);
	cmd_str = ft_calloc(count + 1, sizeof(char *));
	if (!cmd_str)
		return (NULL);
	i = 0;
	fill_cmd_suffix(cmd_str, cmd_suffix, &i);
	cmd_str[i] = NULL;
	return (cmd_str);
}

static char	**build_cmd_str(char *cmd_name, t_ast *cmd_suffix, char *bin_path)
{
	char	**cmd_str;
	char	*tmp_str;
	int		count;
	int		i;

	count = count_cmd_suffix(cmd_suffix) + 1;
	cmd_str = ft_calloc(count + 1, sizeof(char *));
	if (!cmd_str)
		return (NULL);
	if (bin_path[0] != '\0')
		tmp_str = ft_strjoin(bin_path, "/");
	else
		tmp_str = ft_strdup("");
	cmd_str[0] = ft_strjoin(tmp_str, cmd_name);
	free(tmp_str);
	i = 1;
	fill_cmd_suffix(cmd_str, cmd_suffix, &i);
	cmd_str[i] = NULL;
	return (cmd_str);
}

static char	**handle_empty_name(t_ast *cmd_suffix, t_shell_env *env)
{
	char	**cmd_s;
	char	**exp_s;

	if (!cmd_suffix)
		return (NULL);
	cmd_s = build_suffix_only(cmd_suffix);
	if (!cmd_s)
		return (NULL);
	exp_s = expand_args(cmd_s, env);
	free_cmd_str(cmd_s);
	return (exp_s);
}

char	**get_cmd_str(char *cmd_name, t_ast *cmd_suffix, t_shell_env *env)
{
	char			**cmd_s;
	char			**exp_s;
	char			*exp_n;
	t_expand_ctx	*ctx;
	char			*prefix;

	if (!cmd_name)
		return (NULL);
	ctx = create_expand_ctx(env);
	exp_n = expand_string(cmd_name, ctx);
	destroy_expand_ctx(ctx);
	if (!exp_n || exp_n[0] == '\0')
	{
		free(exp_n);
		return (handle_empty_name(cmd_suffix, env));
	}
	prefix = resolve_command_path(exp_n, env);
	cmd_s = build_cmd_str(exp_n, cmd_suffix, prefix);
	free(prefix);
	free(exp_n);
	exp_s = expand_args(cmd_s, env);
	free_cmd_str(cmd_s);
	return (exp_s);
}
