/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:51:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/20 14:58:50 by hermarti         ###   ########.fr       */
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

}

t_parser	*create_parser(t_lexer *lexer)
{
	t_parser	*parser;

	if (lexer == NULL)
		return (NULL);
	parser = (t_parser*) malloc(sizeof(t_parser));
	if (parser == NULL)
		return (NULL);
	parser->lexer = lexer;
	next_token(parser);
	return (parser);
}
