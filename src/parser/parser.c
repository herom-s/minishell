/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:51:10 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/14 10:44:39 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
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

t_ast	*init_ast(t_parser *parser)
{
	return parse_and_or(parser);
}

t_ast	*parsing(t_parser *parser, char *envp[])
{
	t_ast				*ast;
	t_shell_response	*res;

	ast = NULL;
	while (!cur_token_is(parser->cur_token, 1 << END))
	{
		ast = init_ast(parser);
		//print_ast(ast, 0);
		res = eval_ast(ast, envp);
		ft_printf("%s\n", res->output);
		free(parser->cur_token->literal);
		free(parser->cur_token);
		next_token(parser);
	}
	return (ast);
}
