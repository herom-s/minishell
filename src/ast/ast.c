/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 06:54:33 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/22 14:32:44 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "ast.h"
#include "parser.h"
#include "libft.h"
#include <stdio.h>

char	*ast_type_to_str(t_ast_type type)
{
	const char	*ast_literal_list[] = {
	[AST_LIST] = "list",
	[AST_AND_OR] = "and_or",
	[AST_PIPE_SEQ] = "pipe_seq",
	[AST_SUBSHELL] = "subshell",
	[AST_SIMPLE_CMD] = "simple_cmd",
	[AST_CMD_PREFIX] = "cmd_prefix",
	[AST_CMD_SUFFIX] = "cmd_suffix",
	[AST_IO_FILE] = "io_file",
	};

	return ((char *)ast_literal_list[type]);
}

// void	print_ast(t_ast *node, int depth)
// {
// 	int	ident_space;

// 	if (!node)
// 		return ;
// 	ident_space = 2;
// 	if (node->type == AST_AND_OR)
// 	{
// 		printf("%*s<%s op = %s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type), node->u_ast.s_and_or.op->literal);
// 		print_ast(node->u_ast.s_and_or.left, depth + 1);
// 		print_ast(node->u_ast.s_and_or.right, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_PIPE_SEQ)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		print_ast(node->u_ast.s_pipe_seq.left, depth + 1);
// 		print_ast(node->u_ast.s_pipe_seq.right, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_SIMPLE_CMD)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		printf("%*scmd_name: %s\n", ident_space * (depth + 1), "",
// 			node->u_ast.s_simple_cmd.cmd_name);
// 		print_ast(node->u_ast.s_simple_cmd.cmd_prefix, depth + 1);
// 		print_ast(node->u_ast.s_simple_cmd.cmd_suffix, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_CMD_PREFIX)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		print_ast(node->u_ast.s_cmd_prefix.io_file, depth + 1);
// 		print_ast(node->u_ast.s_cmd_prefix.cmd_prefix, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_CMD_SUFFIX)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		print_ast(node->u_ast.s_cmd_suffix.io_file, depth + 1);
// 		if (node->u_ast.s_cmd_suffix.word)
// 			printf("%*sword: %s\n", ident_space * (depth + 1), "",
// 				node->u_ast.s_cmd_suffix.word);
// 		print_ast(node->u_ast.s_cmd_suffix.cmd_suffix, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_IO_FILE)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		printf("%*soperation: %s\n", ident_space * (depth + 1), "",
// 			node->u_ast.s_io_file.op->literal);
// 		printf("%*sfilename: %s\n", ident_space * (depth + 1), "",
// 			node->u_ast.s_io_file.filename);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// 	else if (node->type == AST_SUBSHELL)
// 	{
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 		print_ast(node->u_ast.s_subshell.and_or, depth + 1);
// 		printf("%*s<%s>\n", ident_space * depth, "",
// 			ast_type_to_str(node->type));
// 	}
// }

void	*free_ast(t_ast *node)
{
	if (node == NULL)
		return (NULL);
	if (node->type == AST_AND_OR || node->type == AST_PIPE_SEQ)
		free_left_right_node(node);
	else if (node->type == AST_SUBSHELL)
		free_ast(node->u_ast.s_subshell.and_or);
	else if (node->type == AST_SIMPLE_CMD)
		free_simple_cmd(node);
	else if (node->type == AST_CMD_PREFIX)
		free_cmd_prefix(node);
	else if (node->type == AST_CMD_SUFFIX)
		free_cmd_suffix(node);
	free(node);
	return (NULL);
}

t_ast	*create_ast(t_ast node)
{
	t_ast	*ast_node;

	ast_node = ft_calloc(1, sizeof(t_ast));
	if (ast_node == NULL)
		return (NULL);
	*ast_node = node;
	return (ast_node);
}

t_ast	*init_ast(t_parser *parser)
{
	t_ast	*root;

	root = parse_and_or(parser);
	if (cur_token_is(parser->cur_token, (1 << RPAREN))
		|| !cur_token_is(parser->cur_token, (1 << END)))
	{
		parser_error(parser);
		return (free_ast(root));
	}
	return (root);
}
