/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:37:26 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/02 18:37:27 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"
#include "token.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	has_redirections(t_ast *shell_ast)
{
	t_ast	*suffix;

	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		if (suffix->u_ast.s_cmd_suffix.io_file)
			return (1);
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

static int	open_dup2_io_file(const char *filename, t_token_type op)
{
	int	fd;

	fd = -1;
	if (op == GREAT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (op == LESS)
		fd = open(filename, O_RDONLY);
	else if (op == DGREAT)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", filename, strerror(errno));
		return (-1);
	}
	if (op == LESS || op == DLESS)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	eval_redir(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*io_file;
	int		fd;

	fd = -1;
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		io_file = suffix->u_ast.s_cmd_suffix.io_file;
		if (io_file)
		{
			if (open_dup2_io_file(io_file->u_ast.s_io_file.filename,
					io_file->u_ast.s_io_file.op->type) < 0)
				return (-1);
		}
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (1);
}
