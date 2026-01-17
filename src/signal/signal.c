/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 08:08:43 by thaperei          #+#    #+#             */
/*   Updated: 2026/01/15 17:57:22 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <readline/readline.h>

volatile sig_atomic_t	g_is_sigint_received = 0;

void	handle_ctrl_c(int sig)
{
	(void)sig;
	g_is_sigint_received = 1;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_is_sigint_received = 1;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	setup_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror(strerror(errno));
		return (-1);
	}
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	setup_heredoc_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_heredoc_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror(strerror(errno));
		return (-1);
	}
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
