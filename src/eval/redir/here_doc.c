/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:54:09 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 19:01:18 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	exec_heredoc(char *limiter, int *heredoc_fd)
{
	int		pipefd[2];
	char	*line;
	size_t	n;

	if (pipe(pipefd) < 0)
		return (-1);
	while (!g_is_sigint_received)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			if (g_is_sigint_received)
			{
				close(pipefd[1]);
				close(pipefd[0]);
				return (-1);
			}
			ft_dprintf(STDERR_FILENO,
				"minishell: warning: here-document delimited by end-of-file (\
				%s )\n", limiter);
			break ;
		}
		n = ft_strlen(line);
		if (n > 0 && line[n - 1] == '\n')
			line[n - 1] = '\0';
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	*heredoc_fd = pipefd[0];
	return (0);
}
