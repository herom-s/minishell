/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:51:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/27 20:43:03 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include <stdlib.h>

t_ast	*read_input(char *input)
{
	t_ast	*ast;

	ast = NULL;
	(void)input;
	return (ast);
}

void next_token(t_parser *parser)
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
	*parser = (t_parser) {lexer, NULL, NULL};
	next_token(parser);
	next_token(parser);
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
