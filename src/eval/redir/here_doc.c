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

#include "libft.h"
#include "eval.h"
#include <unistd.h>
#include <stdlib.h>

static int	is_limiter_match(char *line, char *limiter, int limiter_len)
{
	return (ft_strncmp(line, limiter, limiter_len) == 0
		&& line[limiter_len] == '\n');
}

int	handle_here_doc(char *limiter, int *heredoc_fd)
{
	int		pipe_fd[2];
	int		limiter_len;
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			close(pipe_fd[INPUT_END]);
			close(pipe_fd[OUTPUT_END]);
			get_next_line(-1);
			break ;
		}
		if (is_limiter_match(line, limiter, limiter_len))
		{
			free(line);
			break ;
		}
		write(pipe_fd[OUTPUT_END], line, ft_strlen(line));
		free(line);
	}
	get_next_line(-1);
	close(pipe_fd[OUTPUT_END]);
	*heredoc_fd = pipe_fd[INPUT_END];
	return (0);
}
