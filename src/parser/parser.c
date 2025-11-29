/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:51:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/29 13:39:08 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"
#include <stdlib.h>

t_ast	*parse(t_parser *parser)
{
	t_ast	*ast;

	ast = NULL;
	while (parser->cur_token->type != END)
	{
		#code
	}
	return (ast);
}

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
	*parser = (t_parser){lexer, NULL, NULL};
	next_token(parser);
	next_token(parser);
	return (parser);
}

void	free_parser(t_parser *parser)
{
	free(parser);
}
