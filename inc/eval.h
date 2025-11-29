/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:21:40 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/27 20:34:35 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "ast.h"

typedef struct s_shell_response
{
	char			*output;
	int				exit_code;
	char			*erro_msg;
}					t_shell_response;

t_shell_response	*eval_ast(t_ast *shell_ast);

#endif
