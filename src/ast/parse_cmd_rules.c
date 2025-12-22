/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:43:26 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/22 16:25:25 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

t_ast	*parse_subshell(t_parser *parser)
{
	t_ast	node;
	t_ast	*and_or;

	and_or = parse_and_or(parser);
	if (!cur_token_is(parser->cur_token, (1 << RPAREN)))
	{
		parser_error(parser);
		return (NULL);
	}
	next_token(parser);
	node = (t_ast){.type = AST_SUBSHELL, .u_ast.s_subshell.and_or = and_or};
	return (create_ast(node));
}

t_ast	*parse_io_redirect(t_parser *parser)
{
	t_ast	node;
	t_token	*op;
	char	*filename;

	if (!cur_token_is(parser->cur_token, (1 << GREAT) | (1 << DGREAT)
			| (1 << LESS) | (1 << DLESS)))
		return (NULL);
	op = parser->cur_token;
	next_token(parser);
	if (!cur_token_is(parser->cur_token, 1 << WORD))
	{
		parser_error(parser);
		return (NULL);
	}
	filename = parser->cur_token->literal;
	next_token(parser);
	node = (t_ast){.type = AST_IO_FILE, .u_ast.s_io_file.filename = filename,
		.u_ast.s_io_file.op = op};
	return (create_ast(node));
}
