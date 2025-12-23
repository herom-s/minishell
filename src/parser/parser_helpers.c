/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 05:44:00 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/23 13:21:56 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

int	cur_token_is(t_token *token, t_token_type expected)
{
	return ((1 << token->type) & expected);
}

int	peek_token_is(t_parser *parser, t_token_type expected)
{
	return ((1 << parser->peek_token->type) & expected);
}

void	*parser_error(t_parser *parser)
{
	parser->has_error = 1;
	ft_dprintf(2, "Minishell: Syntax error near unexpected token '%s'\n",
		parser->cur_token->literal);
	return (NULL);
}
