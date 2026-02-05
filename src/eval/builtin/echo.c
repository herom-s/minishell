/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:37:48 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 17:09:47 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>

static int	is_newline_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || !arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	print_echo_args(char **cmd_str, int start_idx, int newline)
{
	int	i;

	i = start_idx;
	while (cmd_str[i])
	{
		ft_dprintf(STDOUT_FILENO, "%s", cmd_str[i]);
		if (cmd_str[i + 1])
			ft_dprintf(STDOUT_FILENO, " ");
		i++;
	}
	if (newline)
		ft_dprintf(STDOUT_FILENO, "\n");
}

t_cmd_response	*func_built_in_echo(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	int				i;
	int				newline;

	(void)shell_ast;
	(void)env;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	i = 1;
	newline = 1;
	while (cmd_str[i] && is_newline_flag(cmd_str[i]))
	{
		newline = 0;
		i++;
	}
	print_echo_args(cmd_str, i, newline);
	res->exit_code = 0;
	return (res);
}
