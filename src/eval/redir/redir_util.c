/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 17:49:07 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 13:26:35 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "token.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	has_redirections(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*prefix;

	prefix = shell_ast->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix)
	{
		if (prefix->u_ast.s_cmd_prefix.io_file)
			return (1);
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	}
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		if (suffix->u_ast.s_cmd_suffix.io_file)
			return (1);
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

int	get_fd_for_op(const char *filename, t_token_type op)
{
	if (op == GREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (op == LESS)
		return (open(filename, O_RDONLY));
	if (op == DGREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}
