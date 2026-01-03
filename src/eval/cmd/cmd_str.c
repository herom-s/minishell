/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:47:15 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/07 08:56:01 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
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

static char	**create_fallback_cmd(char *cmd_name)
{
	char	**cmd_str;

	cmd_str = ft_calloc(2, sizeof(char *));
	if (!cmd_str)
		return (NULL);
	cmd_str[0] = ft_strdup(cmd_name);
	cmd_str[1] = NULL;
	return (cmd_str);
}

char	**get_cmd_str(char *cmd_name, t_ast *cmd_suffix, char *envp[])
{
	char	**cmd_str;
	char	**bin_paths;
	int		i;

	if (check_builtin(cmd_name))
		return (build_cmd_str(cmd_name, cmd_suffix, ""));
	bin_paths = get_bin_paths(envp);
	i = 0;
	while (bin_paths[i])
	{
		if (check_path(bin_paths[i], cmd_name))
		{
			cmd_str = build_cmd_str(cmd_name, cmd_suffix, bin_paths[i]);
			free_cmd_str(bin_paths);
			return (cmd_str);
		}
		i++;
	}
	free_cmd_str(bin_paths);
	return (create_fallback_cmd(cmd_name));
}
