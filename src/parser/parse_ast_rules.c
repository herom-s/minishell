/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:40:57 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/06 17:25:03 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

t_ast	*parse_and_or(t_parser *parser)
{
	t_ast	node;
	t_ast	*left;
	t_ast	*right;
	t_token	*op;

	left = parse_pipe_sequence(parser);
	if (!cur_token_is(parser->cur_token->type, (1 << AND_IF) | (1 << OR_IF)))
		return (left);
	op = parser->cur_token;
	next_token(parser);
	if (cur_token_is(parser->cur_token->type, (1 << AND_IF) | (1 << OR_IF)
			| (1 << PIPE)))
	{
		parse_error(parser);
		return (NULL);
	}
	right = parse_and_or(parser);
	node = (t_ast){.type = AST_AND_OR, .left = left, .op = op, .right = right};
	return (create_ast(parent));
}

t_ast	*parse_pipe_sequence(t_parser *parser)
{
	t_ast	node;
	t_ast	*left;
	t_ast	*right;

	if (cur_token_is(parser->cur_token->type, (1 << LPAREN)))
		return parse_subshell(parser);
	left = parse_simple_cmd(parser);
	if (!cur_token_is(parse->cur_token->type, (1 << PIPE) | (1 << LPAREN)))
		return (left);
	next_token(parse);
	if (cur_token_is(parse->cur_token->type, (1 << PIPE) | (1 << AND_IF)
			| (1 << OR_IF)))
	{
		parse_error(parser);
		return (NULL);
	}
	right = parse_pipe_sequence(parser);
	node = (t_ast){.type = AST_PIPE_SEQ, .left = left, .right = right};
	return (create_ast(parent));
}

t_ast	*parse_simple_cmd(t_parser *parser)
{
	t_ast	node;
	t_ast	*cmd_prefix;
	t_ast	*cmd_suffix;

	cmd_prefix = parse_cmd_prefix(parser);
	node.cmd_name = parser->cur_token->literal;
	next_token(parser);
	cmd_suffix = parse_cmd_suffix(parser);
	node = (t_ast){.cmd_prefix = cmd_prefix, .cmd_suffix = cmd_suffix};
	return create_ast(node);
}

t_ast	*parse_cmd_prefix(t_parser *parser)
{
	t_ast	node;
	t_ast	*io_file;
	t_ast	*cmd_prefix;

	cmd_prefix = NULL;
	io_file = parse_io_redirect(parser);
	if (peek_token_is(parser, (1 << GREAT) | (1 << DGREAT)
			| (1 << LESS) | (1 << DLESS)))
		cmd_prefix = parse_io_redirect(parser);
	node = (t_ast){.type = AST_CMD_PREFIX, .io_file = io_file,
		.cmd_prefix = cmd_prefix};
	return create_ast(node);
}

t_ast	*parse_cmd_suffix(t_parser *parser)
{
	t_ast	node;
	t_ast	*io_file;
	t_ast	*cmd_prefix;

	cmd_prefix = parse_cmd_prefix(parser);
	node.word = parser->cur_token->literal;
	next_token(parser);
	cmd_suffix = parse_cmd_suffix(parser);
	node = (t_ast){.cmd_prefix = cmd_prefix, .cmd_suffix = cmd_suffix}
	return create_ast(node);
}
