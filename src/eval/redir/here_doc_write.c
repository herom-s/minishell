/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 15:35:01 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:35:26 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "expand.h"
#include "libft.h"
#include "minishell_signal.h"
#include <readline/readline.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static char	*expand_heredoc_line(char *line, t_shell_env *env, int expand)
{
	t_expand_ctx	ctx;
	char			*expanded;

	if (!expand || !line)
		return (line);
	ctx.env = env;
	ctx.quote_state = QUOTE_NONE;
	expanded = expand_string(line, &ctx);
	free(line);
	return (expanded);
}

int	write_heredoc_to_file(char *delim, char *filename, t_shell_env *env,
		int expand)
{
	int		fd;
	char	*line;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = read_line_interactive();
		else
			line = read_line_noninteractive();
		if (should_stop_heredoc(line, delim))
		{
			free(line);
			break ;
		}
		line = expand_heredoc_line(line, env, expand);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	if (g_sig == SIGINT)
		return (-1);
	return (0);
}

static int	delimiter_is_quoted(const char *delim)
{
	if (!delim)
		return (0);
	while (*delim)
	{
		if (*delim == '\'' || *delim == '"')
			return (1);
		delim++;
	}
	return (0);
}

int	handle_heredoc_io(t_ast *io, t_shell_env *env)
{
	char	*temp_file;
	char	*delim;
	int		expand;

	if (io && io->u_ast.s_io_file.op->type == DLESS)
	{
		delim = (char *)io->u_ast.s_io_file.filename;
		expand = !delimiter_is_quoted(delim);
		if (expand == 0)
			delim = remove_quotes(delim);
		temp_file = generate_heredoc_filename();
		if (write_heredoc_to_file(delim, temp_file, env, expand) == -1)
		{
			if (expand == 0)
				free(delim);
			free(temp_file);
			return (-1);
		}
		if (expand == 0)
			free(delim);
		io->u_ast.s_io_file.op->type = LESS;
		io->u_ast.s_io_file.filename = temp_file;
	}
	return (0);
}
