/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 06:54:33 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/07 09:05:30 by thaperei         ###   ########.fr       */
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
		[AST_PIPE_SEQ] "pipe_seq",
		[AST_SUBSHELL] = "subshell",
		[AST_SIMPLE_CMD] = "simple_cmd",
		[AST_CMD_PREFIX] = "cmd_prefix",
		[AST_CMD_SUFFIX] = "cmd_suffix",
		[AST_IO_FILE] = "io_file",
	};

	return ((char *)ast_literal_list[type]);
}

void	print_ast(t_ast *node, int depth)
{
	int	ident_space;

	if (!node)
	 	return ;
	ident_space = 2;
	if (node->type == AST_AND_OR)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_and_or.left, depth + 1);
		print_ast(node->u_ast.s_and_or.right, depth + 1);
	}
	else if (node->type == AST_PIPE_SEQ)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_pipe_seq.left, depth + 1);
		print_ast(node->u_ast.s_pipe_seq.right, depth + 1);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
	else if (node->type == AST_SIMPLE_CMD)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_simple_cmd.cmd_prefix, depth + 1);
		printf("cmd_name: %s\n", node->u_ast.s_simple_cmd.cmd_name);
		print_ast(node->u_ast.s_simple_cmd.cmd_suffix, depth + 1);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
	else if (node->type == AST_CMD_PREFIX)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_cmd_prefix.cmd_prefix, depth + 1);
		print_ast(node->u_ast.s_cmd_prefix.io_file, depth + 1);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
	else if (node->type == AST_CMD_SUFFIX)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_cmd_suffix.io_file, depth + 1);
		printf("word: %s\n", node->u_ast.s_cmd_suffix.word);
		print_ast(node->u_ast.s_cmd_suffix.cmd_suffix, depth + 1);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
	else if (node->type == AST_IO_FILE)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		printf("operation: %s\n", node->u_ast.s_io_file.op->literal);
		printf("filename: %s\n", node->u_ast.s_io_file.filename);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
	else if (node->type == AST_SUBSHELL)
	{
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
		print_ast(node->u_ast.s_subshell.and_or, depth + 1);
		printf("%*s<%s>\n", ident_space * depth, "", ast_type_to_str(node->type));
	}
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
