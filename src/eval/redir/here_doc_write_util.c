/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_write_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 13:25:56 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:36:14 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <readline/readline.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

char	*generate_heredoc_filename(void)
{
	static int	i = 0;
	char		*num;
	char		*name;

	num = ft_itoa(i++);
	name = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	return (name);
}

void	print_eof_warning(char *delimiter)
{
	if (!g_sig && isatty(STDERR_FILENO))
		ft_dprintf(STDERR_FILENO,
			"minishell: warning: here-document delimited by "
			"end-of-file (wanted `%s')\n",
			delimiter);
}

char	*read_line_interactive(void)
{
	char	*line;

	rl_event_hook = check_signal;
	line = readline("> ");
	rl_event_hook = NULL;
	return (line);
}

char	*read_line_noninteractive(void)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

int	should_stop_heredoc(char *line, char *delimiter)
{
	if (g_sig == SIGINT || !line)
	{
		if (!line)
			print_eof_warning(delimiter);
		return (1);
	}
	if (ft_strcmp(line, delimiter) == 0)
		return (1);
	return (0);
}
