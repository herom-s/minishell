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

#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <readline/readline.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int	write_heredoc_to_file(char *delimiter, char *filename)
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
		if (should_stop_heredoc(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	if (g_sig == SIGINT)
		return (-1);
	return (0);
}
