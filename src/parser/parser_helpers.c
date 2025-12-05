/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 05:44:00 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/05 07:18:50 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

int	cur_token_is(t_token token, t_token_type expected)
{
	return ((1 << token.type) & expected);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}

void	parser_error(t_parser *parser)
{
	parser->has_error = 1;
	ft_printf("Syntax error near unexpected token '%s'",
		parser->cur_token->literal);
}
