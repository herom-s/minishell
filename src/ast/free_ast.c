/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 10:21:17 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/14 10:29:04 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

void	free_left_right_node(t_ast *node)
{
	if (node->type == AST_AND_OR)
	{
		free_ast(node->u_ast.s_and_or.left);
		free_ast(node->u_ast.s_and_or.right);
	}
	else
	{
		free_ast(node->u_ast.s_pipe_seq.left);
		free_ast(node->u_ast.s_pipe_seq.right);
	}
}

void	free_simple_cmd(t_ast *node)
{
	free_ast(node->u_ast.s_simple_cmd.cmd_prefix);
	free_ast(node->u_ast.s_simple_cmd.cmd_suffix);
}

void	free_cmd_prefix(t_ast *node)
{
	free_ast(node->u_ast.s_cmd_prefix.cmd_prefix);
	free_ast(node->u_ast.s_cmd_prefix.io_file);
}

void	free_cmd_suffix(t_ast *node)
{
	free_ast(node->u_ast.s_cmd_suffix.io_file);
	free_ast(node->u_ast.s_cmd_suffix.cmd_suffix);
}
