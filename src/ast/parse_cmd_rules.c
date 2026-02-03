/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:43:26 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/23 16:05:52 by thaperei         ###   ########.fr       */
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
		return (free_ast(and_or));
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
		return (parser_error(parser));
	filename = parser->cur_token->literal;
	next_token(parser);
	node = (t_ast){.type = AST_IO_FILE, .u_ast.s_io_file.filename = filename,
		.u_ast.s_io_file.op = op};
	return (create_ast(node));
}

t_ast	*parse_cmd_prefix(t_parser *parser)
{
	t_ast	*io_file;
	t_ast	*cmd_prefix;

	io_file = parse_io_redirect(parser);
	if (!io_file)
		return (NULL);
	cmd_prefix = parse_cmd_prefix(parser);
	return (create_ast((t_ast){.type = AST_CMD_PREFIX,
			.u_ast.s_cmd_prefix.io_file = io_file,
			.u_ast.s_cmd_prefix.cmd_prefix = cmd_prefix}));
}

t_ast	*parse_cmd_suffix(t_parser *parser)
{
	t_ast	node;

	node.type = AST_CMD_SUFFIX;
	node.u_ast.s_cmd_suffix.io_file = parse_io_redirect(parser);
	if (node.u_ast.s_cmd_suffix.io_file)
	{
		node.u_ast.s_cmd_suffix.cmd_suffix = parse_cmd_suffix(parser);
		node.u_ast.s_cmd_suffix.word = NULL;
		return (create_ast(node));
	}
	if (!cur_token_is(parser->cur_token, (1 << WORD)))
		return (NULL);
	node.u_ast.s_cmd_suffix.word = parser->cur_token->literal;
	next_token(parser);
	node.u_ast.s_cmd_suffix.cmd_suffix = parse_cmd_suffix(parser);
	if (parser->has_error)
		return (NULL);
	return (create_ast(node));
}
