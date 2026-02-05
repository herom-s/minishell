/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:37:26 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "expand.h"
#include "libft.h"
#include "token.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int	setup_redir_dup(int fd, t_token_type op)
{
	int	target;

	if (op == LESS)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	if (dup2(fd, target) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_file_redir(t_ast *io, t_shell_env *env)
{
	int				fd;
	char			*expanded;

	expanded = expand_redir_target(io->u_ast.s_io_file.filename, env);
	if (!expanded)
		return (-1);
	fd = get_fd_for_op(expanded, io->u_ast.s_io_file.op->type);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", expanded, strerror(errno));
		free(expanded);
		return (-1);
	}
	free(expanded);
	return (setup_redir_dup(fd, io->u_ast.s_io_file.op->type));
}

static int	process_list(t_ast *node, int is_suffix, t_shell_env *env)
{
	t_ast	*io;

	while (node)
	{
		if (is_suffix)
			io = node->u_ast.s_cmd_suffix.io_file;
		else
			io = node->u_ast.s_cmd_prefix.io_file;
		if (io)
		{
			if (handle_file_redir(io, env) < 0)
				return (-1);
		}
		if (is_suffix)
			node = node->u_ast.s_cmd_suffix.cmd_suffix;
		else
			node = node->u_ast.s_cmd_prefix.cmd_prefix;
	}
	return (0);
}

int	eval_redir(t_ast *shell_ast, t_shell_env *env)
{
	int	saved_stdin;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (-1);
	ret = process_list(shell_ast->u_ast.s_simple_cmd.cmd_prefix, 0, env);
	if (ret != -1)
		ret = process_list(shell_ast->u_ast.s_simple_cmd.cmd_suffix, 1, env);
	if (ret < 0)
	{
		close(saved_stdin);
		return (-1);
	}
	close(saved_stdin);
	return (1);
}

int	eval_io_file(t_ast *io_file, t_shell_env *env)
{
	if (!io_file)
		return (1);
	if (handle_file_redir(io_file, env) < 0)
		return (-1);
	return (1);
}
