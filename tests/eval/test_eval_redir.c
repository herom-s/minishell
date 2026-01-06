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
 * Finds the command node among the nodes array and attaches
 * redirections as prefix (before cmd) or suffix (after cmd)
 */
t_ast	*create_redirection_ast(t_ast **nodes, int count)
{
	t_ast	*cmd_node;
	t_ast	*last_prefix;
	t_ast	*last_suffix;
	t_ast	*new_node;
	int		i;
	int		cmd_found;

	if (count < 1 || !nodes)
		return (NULL);
	cmd_node = NULL;
	cmd_found = 0;
	i = 0;
	while (i < count)
	{
		if (nodes[i] && nodes[i]->type == AST_SIMPLE_CMD)
		{
			cmd_node = nodes[i];
			cmd_found = i;
			break ;
		}
		i++;
	}
	if (!cmd_node)
		return (NULL);
	last_prefix = NULL;
	i = 0;
	while (i < cmd_found)
	{
		if (nodes[i] && nodes[i]->type == AST_IO_FILE)
		{
			new_node = calloc(1, sizeof(t_ast));
			if (!new_node)
				return (NULL);
			new_node->type = AST_CMD_PREFIX;
			new_node->u_ast.s_cmd_prefix.io_file = nodes[i];
			if (last_prefix)
				last_prefix->u_ast.s_cmd_prefix.cmd_prefix = new_node;
			else
				cmd_node->u_ast.s_simple_cmd.cmd_prefix = new_node;
			last_prefix = new_node;
		}
		i++;
	}
	last_suffix = cmd_node->u_ast.s_simple_cmd.cmd_suffix;
	while (last_suffix && last_suffix->u_ast.s_cmd_suffix.cmd_suffix)
		last_suffix = last_suffix->u_ast.s_cmd_suffix.cmd_suffix;
	i = cmd_found + 1;
	while (i < count)
	{
		if (nodes[i] && nodes[i]->type == AST_IO_FILE)
		{
			new_node = calloc(1, sizeof(t_ast));
			if (!new_node)
				return (NULL);
			new_node->type = AST_CMD_SUFFIX;
			new_node->u_ast.s_cmd_suffix.io_file = nodes[i];
			if (last_suffix)
				last_suffix->u_ast.s_cmd_suffix.cmd_suffix = new_node;
			else
				cmd_node->u_ast.s_simple_cmd.cmd_suffix = new_node;
			last_suffix = new_node;
		}
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
	t_token	*token;

	node = calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	token = calloc(1, sizeof(t_token));
	if (!token)
	{
		free(node);
		return (NULL);
	}
	token->type = type;
	token->literal = NULL;
	token->len = 0;
	node->type = AST_IO_FILE;
	node->u_ast.s_io_file.op = token;
	node->u_ast.s_io_file.filename = filename;
	return (node);
}

/*
 *   helper: free a single io_file node completely
 */
static void	free_io_file_node(t_ast *io_file)
{
	if (!io_file)
		return ;
	if (io_file->u_ast.s_io_file.op)
		free_token(io_file->u_ast.s_io_file.op);
	free(io_file);
}

/*
 *   teardown: free redir AST
 */
int	teardown_free_redir_ast(void **state)
{
	t_ast	*ast;
	t_ast	*prefix;
	t_ast	*suffix;
	t_ast	*next;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	if (ast->type == AST_SIMPLE_CMD)
	{
		prefix = ast->u_ast.s_simple_cmd.cmd_prefix;
		while (prefix)
		{
			next = prefix->u_ast.s_cmd_prefix.cmd_prefix;
			free_io_file_node(prefix->u_ast.s_cmd_prefix.io_file);
			free(prefix);
			prefix = next;
		}
		suffix = ast->u_ast.s_simple_cmd.cmd_suffix;
		while (suffix)
		{
			next = suffix->u_ast.s_cmd_suffix.cmd_suffix;
			free_io_file_node(suffix->u_ast.s_cmd_suffix.io_file);
			free(suffix);
			suffix = next;
		}
	}
	free(ast);
	*state = NULL;
	return (0);
}
