/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:10:16 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/17 15:10:57 by hermarti         ###   ########.fr       */
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
