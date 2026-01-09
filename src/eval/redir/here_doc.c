/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:54:09 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/09 14:00:18 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static int	is_limiter_match(char *line, char *limiter, int limiter_len)
{
	return (ft_strncmp(line, limiter, limiter_len) == 0
		&& line[limiter_len] == '\n');
}

static char	*read_line_from_fd(int fd)
{
	char	*line;
	char	*tmp;
	char	buf[2];
	int		bytes;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	buf[1] = '\0';
	while (1)
	{
		bytes = read(fd, buf, 1);
		if (bytes <= 0)
		{
			if (ft_strlen(line) == 0)
			{
				free(line);
				return (NULL);
			}
			return (line);
		}
		tmp = ft_strjoin(line, buf);
		free(line);
		line = tmp;
		if (!line)
			return (NULL);
		if (buf[0] == '\n')
			return (line);
	}
}

int	handle_here_doc(char *limiter, int *heredoc_fd, int read_fd)
{
	int		pipe_fd[2];
	int		limiter_len;
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = read_line_from_fd(read_fd);
		if (!line)
		{
			close(pipe_fd[INPUT_END]);
			close(pipe_fd[OUTPUT_END]);
			return (-1);
		}
		if (is_limiter_match(line, limiter, limiter_len))
		{
			free(line);
			break ;
		}
		write(pipe_fd[OUTPUT_END], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[OUTPUT_END]);
	*heredoc_fd = pipe_fd[INPUT_END];
	return (0);
}
