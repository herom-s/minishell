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

static char	**allocate_cmd_str(t_ast *cmd_suffix, int *count)
{
	t_ast	*node;
	char	**cmd_str;

	*count = 1;
	node = cmd_suffix;
	while (node)
	{
		(*count)++;
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	cmd_str = ft_calloc(*count + 1, sizeof(char *));
	return (cmd_str);
}

void	free_cmd_str(char **cmd_str)
{
	int	i;

	if (!cmd_str)
		return ;
	i = 0;
	while (cmd_str[i])
	{
		free(cmd_str[i]);
		i++;
	}
	free(cmd_str);
}

static char	**build_suffix_only(t_ast *cmd_suffix)
{
	t_ast	*node;
	char	**cmd_str;
	int		count;
	int		i;

	count = 0;
	node = cmd_suffix;
	while (node)
	{
		if (node->u_ast.s_cmd_suffix.word)
			count++;
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	if (count == 0)
		return (NULL);
	cmd_str = ft_calloc(count + 1, sizeof(char *));
	if (!cmd_str)
		return (NULL);
	i = 0;
	node = cmd_suffix;
	while (node)
	{
		if (node->u_ast.s_cmd_suffix.word)
			cmd_str[i++] = ft_strdup(node->u_ast.s_cmd_suffix.word);
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	cmd_str[i] = NULL;
	return (cmd_str);
}

static char	**build_cmd_str(char *cmd_name, t_ast *cmd_suffix, char *bin_path)
{
	t_ast	*node;
	char	**cmd_str;
	char	*tmp_str;
	int		count;
	int		i;

	cmd_str = allocate_cmd_str(cmd_suffix, &count);
	if (!cmd_str)
		return (NULL);
	if (bin_path[0] != '\0')
		tmp_str = ft_strjoin(bin_path, "/");
	else
		tmp_str = ft_strdup("");
	cmd_str[0] = ft_strjoin(tmp_str, cmd_name);
	free(tmp_str);
	i = 1;
	node = cmd_suffix;
	while (node)
	{
		if (node->u_ast.s_cmd_suffix.word)
			cmd_str[i++] = ft_strdup(node->u_ast.s_cmd_suffix.word);
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	cmd_str[i] = NULL;
	return (cmd_str);
}

char	**get_cmd_str(char *cmd_name, t_ast *cmd_suffix, t_shell_env *env)
{
	char			**cmd_str;
	char			**expanded_str;
	char			**bin_paths;
	char			*expanded_name;
	t_expand_ctx	*ctx;
	int				i;

	if (!cmd_name)
		return (NULL);
	ctx = create_expand_ctx(env);
	expanded_name = expand_string(cmd_name, ctx);
	destroy_expand_ctx(ctx);
	if (!expanded_name || expanded_name[0] == '\0')
	{
		free(expanded_name);
		if (!cmd_suffix)
			return (NULL);
		cmd_str = build_suffix_only(cmd_suffix);
		if (!cmd_str)
			return (NULL);
		expanded_str = expand_args(cmd_str, env);
		free_cmd_str(cmd_str);
		return (expanded_str);
	}
	if (ft_strchr(expanded_name, '/'))
		cmd_str = build_cmd_str(expanded_name, cmd_suffix, "");
	else if (check_builtin(expanded_name))
		cmd_str = build_cmd_str(expanded_name, cmd_suffix, "");
	else
	{
		bin_paths = get_bin_paths(env);
		i = 0;
		while (bin_paths[i] && !check_path(bin_paths[i], expanded_name))
			i++;
		if (bin_paths[i])
			cmd_str = build_cmd_str(expanded_name, cmd_suffix, bin_paths[i]);
		else
			cmd_str = build_cmd_str(expanded_name, cmd_suffix, "");
		free_cmd_str(bin_paths);
	}
	free(expanded_name);
	expanded_str = expand_args(cmd_str, env);
	free_cmd_str(cmd_str);
	return (expanded_str);
}
