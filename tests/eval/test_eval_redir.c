/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:40:05 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/02 18:42:20 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "test_eval.h"
#include <stdlib.h>
#include <string.h>

/*
 * helper: attach redirection nodes to a command node
 * cmds[0] is the command (AST_SIMPLE_CMD)
 * cmds[1..n] are redirection nodes (AST_IO_FILE)
 */
t_ast	*create_redirection_ast(t_ast **cmds, int count)
{
	t_ast	*cmd_node;
	t_ast	*last_suffix;
	t_ast	*new_suffix;
	int		i;

	if (count < 1 || !cmds || !cmds[0])
		return (NULL);
	cmd_node = cmds[0];
	last_suffix = cmd_node->u_ast.s_simple_cmd.cmd_suffix;
	while (last_suffix && last_suffix->u_ast.s_cmd_suffix.cmd_suffix)
		last_suffix = last_suffix->u_ast.s_cmd_suffix.cmd_suffix;
	i = 1;
	while (i < count)
	{
		new_suffix = calloc(1, sizeof(t_ast));
		if (!new_suffix)
			return (NULL);
		new_suffix->type = AST_CMD_SUFFIX;
		new_suffix->u_ast.s_cmd_suffix.io_file = cmds[i];
		if (last_suffix)
			last_suffix->u_ast.s_cmd_suffix.cmd_suffix = new_suffix;
		else
			cmd_node->u_ast.s_simple_cmd.cmd_suffix = new_suffix;
		last_suffix = new_suffix;
		i++;
	}
	return (cmd_node);
}

/*
 *   helper: create an IO_FILE redirection node
 *   type: GREAT (>), DGREAT (>>), LESS (<)
 */
t_ast	*create_io_file_node(t_token_type type, char *filename)
{
	t_ast	*node;

	node = calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_IO_FILE;
	node->u_ast.s_io_file.op = calloc(1, sizeof(t_token));
	if (!node->u_ast.s_io_file.op)
	{
		free(node);
		return (NULL);
	}
	node->u_ast.s_io_file.op->type = type;
	node->u_ast.s_io_file.filename = strdup(filename);
	return (node);
}

/*
 *   teardown: free redir AST
 */
int	teardown_free_redir_ast(void **state)
{
	t_ast	*ast;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	free_ast(ast);
	*state = NULL;
	return (0);
}
