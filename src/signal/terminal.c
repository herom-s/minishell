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

int	save_terminal_settings(struct termios *saved)
{
	if (saved && isatty(STDIN_FILENO)
		&& tcgetattr(STDIN_FILENO, saved) == 0)
		return (1);
	return (0);
}

void	restore_terminal_settings(struct termios *saved, int was_saved)
{
	if (was_saved && saved && isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSANOW, saved);
}
