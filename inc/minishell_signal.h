/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 19:27:33 by thaperei          #+#    #+#             */
/*   Updated: 2026/01/08 20:31:39 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H

# include <signal.h>

extern volatile sig_atomic_t	g_is_sigint_received;

int								setup_signal(void);
int								setup_heredoc_signal(void);
int								setup_child_signals(void);
#endif
