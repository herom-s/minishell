/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:30:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 16:30:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <termios.h>
#include <unistd.h>

static struct termios	g_termios;

void	save_terminal_settings(void)
{
	if (isatty(STDIN_FILENO))
		tcgetattr(STDIN_FILENO, &g_termios);
}

void	restore_terminal_settings(void)
{
	if (isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSANOW, &g_termios);
}
