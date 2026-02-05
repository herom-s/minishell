/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 05:44:00 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/23 16:50:50 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

int	cur_token_is(t_token *token, t_token_type expected)
{
	return ((1 << token->type) & expected);
}

int	peek_token_is(t_token *token, t_token_type expected)
{
	return ((1 << token->type) & expected);
}

void	*parser_error(t_parser *parser)
{
	parser->has_error = 1;
	ft_dprintf(2, "Minishell: syntax error near unexpected token '%s'\n",
		parser->cur_token->literal);
	return (NULL);
}
