/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:40:57 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/23 14:11:57 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"
#include "parser.h"

t_ast	*parse_and_or(t_parser *parser)
{
	t_ast	node;

	node.type = AST_AND_OR;
	node.u_ast.s_and_or.left = parse_pipe_sequence(parser);
	if (node.u_ast.s_and_or.left == NULL)
		return (NULL);
	while (cur_token_is(parser->cur_token, (1 << AND_IF) | (1 << OR_IF)))
	{
		node.u_ast.s_and_or.op = parser->cur_token;
		next_token(parser);
		if (cur_token_is(parser->cur_token,
				(1 << AND_IF) | (1 << OR_IF) | (1 << PIPE) | (1 << AMPERSAND)))
		{
			parser_error(parser);
			return (free_ast(node.u_ast.s_and_or.left));
		}
		node.u_ast.s_and_or.right = parse_pipe_sequence(parser);
		if (!node.u_ast.s_and_or.right)
			return (free_ast(node.u_ast.s_and_or.left));
		node.u_ast.s_and_or.left = create_ast(node);
	}
	return (node.u_ast.s_and_or.left);
}

t_ast	*parse_pipe_sequence(t_parser *parser)
{
	t_ast	node;
	t_ast	*left;
	t_ast	*right;

	left = parse_simple_cmd(parser);
	if (left == NULL)
		return (NULL);
	while (cur_token_is(parser->cur_token, (1 << PIPE)))
	{
		next_token(parser);
		if (cur_token_is(parser->cur_token,
				(1 << PIPE) | (1 << AND_IF) | (1 << OR_IF) | (1 << NEWLINE)))
		{
			parser_error(parser);
			return (free_ast(left));
		}
		right = parse_simple_cmd(parser);
		if (right == NULL)
			return (NULL);
		node = (t_ast){.type = AST_PIPE_SEQ, .u_ast.s_pipe_seq.left = left,
			.u_ast.s_pipe_seq.right = right};
		left = create_ast(node);
	}
	return (left);
}

static t_ast	*build_simple_cmd(t_ast *prefix, char *name, t_ast *suffix)
{
	return (create_ast((t_ast){.type = AST_SIMPLE_CMD,
			.u_ast.s_simple_cmd.cmd_prefix = prefix,
			.u_ast.s_simple_cmd.cmd_name = name,
			.u_ast.s_simple_cmd.cmd_suffix = suffix}));
}

static t_ast	*parse_prefix_only_cmd(t_parser *parser, t_ast *cmd_prefix)
{
	if (!cmd_prefix && !parser->has_error)
		return (parser_error(parser));
	if (!cmd_prefix)
		return (NULL);
	return (build_simple_cmd(cmd_prefix, NULL, NULL));
}

t_ast	*parse_simple_cmd(t_parser *parser)
{
	t_ast	*cmd_prefix;
	t_ast	*cmd_suffix;
	char	*cmd_name;

	if (cur_token_is(parser->cur_token, (1 << LPAREN)))
	{
		next_token(parser);
		return (parse_subshell(parser));
	}
	cmd_prefix = parse_cmd_prefix(parser);
	if (cur_token_is(parser->cur_token, (1 << NEWLINE)))
		return (parse_prefix_only_cmd(parser, cmd_prefix));
	if (!cur_token_is(parser->cur_token, (1 << WORD)) && !parser->has_error)
		return (parser_error(parser));
	cmd_name = parser->cur_token->literal;
	next_token(parser);
	cmd_suffix = parse_cmd_suffix(parser);
	if (parser->has_error)
		return (NULL);
	return (build_simple_cmd(cmd_prefix, cmd_name, cmd_suffix));
}
