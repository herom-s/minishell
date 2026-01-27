/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 08:08:43 by thaperei          #+#    #+#             */
/*   Updated: 2026/01/26 16:05:26 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <readline/readline.h>

volatile sig_atomic_t	g_sig = 0;

void	handler_sig(int sig)
{
	(void)sig;
	g_sig = SIGINT;
}

int	check_signal(void)
{
	if (g_sig == SIGINT)
	{
		rl_done = 1;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return (0);
}

int	setup_nonfork_signal(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handler_sig;
	sigaction(SIGINT, &sa, NULL);
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror(strerror(errno));
		return (-1);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
	{
		perror(strerror(errno));
		return (-1);
	}
	return (0);
}

int	setup_fork_signal(pid_t pid)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (pid == 0)
		sa.sa_handler = SIG_DFL;
	else
		sa.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}
