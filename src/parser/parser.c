/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:51:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/06 17:13:59 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"
#include <stdlib.h>

void	next_token(t_parser *parser)
{
	parser->cur_token = parser->peek_token;
	parser->peek_token = get_next_token(parser->lexer);
}

t_parser	*create_parser(t_lexer *lexer)
{
	t_parser	*parser;

	if (lexer == NULL)
		return (NULL);
	parser = ft_calloc(sizeof(t_parser), 1);
	if (parser == NULL)
		return (NULL);
	*parser = (t_parser){lexer, NULL, NULL, 0};
	next_token(parser);
	next_token(parser);
	return (parser);
}

t_ast	*parsing(t_parser *parser)
{
	t_ast	*ast;

	ast = NULL;
	while (!cur_token_is(parser->cur_token, 1 << END))
	{
		ft_printf("token %s - %d\n", parser->cur_token->literal,
			parser->cur_token->type);
		free(parser->cur_token->literal);
		free(parser->cur_token);
		next_token(parser);
	}
	return (ast);
}
