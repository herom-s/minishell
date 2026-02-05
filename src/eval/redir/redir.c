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

static int	get_fd_for_op(const char *filename, t_token_type op)
{
	if (op == GREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (op == LESS)
		return (open(filename, O_RDONLY));
	if (op == DGREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

static char	*expand_redir_target(const char *file, t_shell_env *env)
{
	t_expand_ctx	*ctx;
	char			*var_expanded;
	char			*expanded;
	char			**matches;

	ctx = create_expand_ctx(env);
	var_expanded = expand_variables(file, ctx);
	destroy_expand_ctx(ctx);
	if (!var_expanded)
		return (NULL);
	if (has_wildcard(var_expanded))
	{
		expanded = remove_quotes(var_expanded);
		free(var_expanded);
		matches = expand_wildcards(expanded);
		free(expanded);
		if (!matches || !matches[0] || matches[1])
		{
			ft_dprintf(STDERR_FILENO, "minishell: ambiguous redirect\n");
			if (matches)
			{
				while (*matches)
					free(*matches++);
			}
			return (NULL);
		}
		expanded = ft_strdup(matches[0]);
		free(matches[0]);
		free(matches);
		return (expanded);
	}
	expanded = remove_quotes(var_expanded);
	free(var_expanded);
	return (expanded);
}

static int	handle_file_redir(t_ast *io, t_shell_env *env)
{
	int				fd;
	int				target;
	char			*file;
	char			*expanded;

	file = (char *)io->u_ast.s_io_file.filename;
	expanded = expand_redir_target(file, env);
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
	if (io->u_ast.s_io_file.op->type == LESS)
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

static int	process_io_file(t_ast *io_file, t_shell_env *env)
{
	if (!io_file)
		return (-1);
	return (handle_file_redir(io_file, env));
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
			if (process_io_file(io, env) < 0)
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
