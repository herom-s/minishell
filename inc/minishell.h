/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:33:07 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/27 20:36:45 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "eval.h"
# include "ast.h"
# include <signal.h>

typedef struct s_minishell
{
	char				*input;
	int					sig_shell;
	t_ast				*ast_shell;
	t_shell_response	*shell_response;
	struct sigaction	sigaction_shell;
}						t_minishell;

#endif
