/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:40:57 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/13 16:02:35 by thaperei         ###   ########.fr       */
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

	left = NULL;
	right = NULL;
	op = NULL;
	left = parse_pipe_sequence(parser);
	if (!cur_token_is(parser->cur_token, (1 << AND_IF) | (1 << OR_IF)))
		return (left);
	op = parser->cur_token;
	next_token(parser);
	if (cur_token_is(parser->cur_token, (1 << AND_IF) | (1 << OR_IF)
			| (1 << PIPE)))
	{
		parser_error(parser);
		return (NULL);
	}
	right = parse_and_or(parser);
	node = (t_ast){.type = AST_AND_OR, .u_ast.s_and_or.left = left,
		.u_ast.s_and_or.op = op, .u_ast.s_and_or.right = right};
	return (create_ast(node));
}

t_ast	*parse_pipe_sequence(t_parser *parser)
{
	t_ast	node;
	t_ast	*left;
	t_ast	*right;

	if (cur_token_is(parser->cur_token, (1 << LPAREN)))
	{
		next_token(parser);
		return (parse_subshell(parser));
	}
	left = parse_simple_cmd(parser);
	if (!cur_token_is(parser->cur_token, (1 << PIPE)))
		return (left);
	next_token(parser);
	if (cur_token_is(parser->cur_token, (1 << PIPE) | (1 << AND_IF)
			| (1 << OR_IF)))
	{
		parser_error(parser);
		return (NULL);
	}
	right = parse_pipe_sequence(parser);
	node = (t_ast){.type = AST_PIPE_SEQ, .u_ast.s_pipe_seq.left = left,
		.u_ast.s_pipe_seq.right = right};
	return (create_ast(node));
}

t_ast	*parse_simple_cmd(t_parser *parser)
{
	t_ast	node;
	t_ast	*cmd_prefix;
	t_ast	*cmd_suffix;
	char	*cmd_name;

	cmd_prefix = NULL;
	cmd_suffix = NULL;
	cmd_name = NULL;
	if (cur_token_is(parser->cur_token, ((1 << GREAT) | (1 << DGREAT)
				| (1 << LESS) | (1 << DLESS))))
		cmd_prefix = parse_cmd_prefix(parser);
	if (cur_token_is(parser->cur_token, (1 << WORD)))
	{
		cmd_name = parser->cur_token->literal;
		next_token(parser);
	}
	if (cur_token_is(parser->cur_token, ((1 << GREAT) | (1 << DGREAT)
				| (1 << LESS) | (1 << DLESS) | (1 << WORD))))
		cmd_suffix = parse_cmd_suffix(parser);
	node = (t_ast){.type = AST_SIMPLE_CMD,
		.u_ast.s_simple_cmd.cmd_prefix = cmd_prefix,
		.u_ast.s_simple_cmd.cmd_name = cmd_name,
		.u_ast.s_simple_cmd.cmd_suffix = cmd_suffix};
	return (create_ast(node));
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
		cmd_prefix = parse_cmd_prefix(parser);
	node = (t_ast){.type = AST_CMD_PREFIX,
		.u_ast.s_cmd_prefix.io_file = io_file,
		.u_ast.s_cmd_prefix.cmd_prefix = cmd_prefix};
	return (create_ast(node));
}

t_ast	*parse_cmd_suffix(t_parser *parser)
{
	t_ast		node;
	t_ast		*io_file;
	t_ast		*cmd_suffix;
	const char	*word;

	io_file = parse_io_redirect(parser);
	if (io_file)
	{
		cmd_suffix = parse_cmd_suffix(parser);
		node = (t_ast){.type = AST_CMD_SUFFIX,
			.u_ast.s_cmd_suffix.io_file = io_file,
			.u_ast.s_cmd_suffix.word = NULL,
			.u_ast.s_cmd_suffix.cmd_suffix = cmd_suffix};
		return create_ast(node);
	}
	if (!cur_token_is(parser->cur_token, (1 << WORD)))
		return (NULL);
	word = parser->cur_token->literal;
	next_token(parser);
	cmd_suffix = parse_cmd_suffix(parser);
	node = (t_ast){.type = AST_CMD_SUFFIX,
		.u_ast.s_cmd_suffix.io_file = io_file,
		.u_ast.s_cmd_suffix.word = word,
		.u_ast.s_cmd_suffix.cmd_suffix = cmd_suffix};
	return (create_ast(node));
}
