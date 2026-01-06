/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval_pipe_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 16:46:59 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/05 16:47:24 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "test_eval.h"
#include <stdlib.h>
#include <string.h>

/*
 * helper: create a pipeline with redirections
 * Creates AST_PIPE_SEQ nodes linking commands, where commands may have
 * redirections attached via cmd_suffix
 */
t_ast	*create_pipe_with_redir(t_ast *left, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = calloc(1, sizeof(t_ast));
	if (!pipe_node)
		return (NULL);
	pipe_node->type = AST_PIPE_SEQ;
	pipe_node->u_ast.s_pipe_seq.left = left;
	pipe_node->u_ast.s_pipe_seq.right = right;
	return (pipe_node);
}

/*
 * helper: attach a single redirection to a command as suffix (after cmd)
 */
void	attach_redir_suffix(t_ast *cmd, t_ast *redir)
{
	t_ast	*suffix;
	t_ast	*new_suffix;

	new_suffix = calloc(1, sizeof(t_ast));
	if (!new_suffix)
		return ;
	new_suffix->type = AST_CMD_SUFFIX;
	new_suffix->u_ast.s_cmd_suffix.io_file = redir;
	new_suffix->u_ast.s_cmd_suffix.cmd_suffix = NULL;
	if (!cmd->u_ast.s_simple_cmd.cmd_suffix)
	{
		cmd->u_ast.s_simple_cmd.cmd_suffix = new_suffix;
		return ;
	}
	suffix = cmd->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix->u_ast.s_cmd_suffix.cmd_suffix)
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	suffix->u_ast.s_cmd_suffix.cmd_suffix = new_suffix;
}

/*
 * helper: attach a single redirection to a command as prefix (before cmd)
 */
void	attach_redir_prefix(t_ast *cmd, t_ast *redir)
{
	t_ast	*prefix;
	t_ast	*new_prefix;

	new_prefix = calloc(1, sizeof(t_ast));
	if (!new_prefix)
		return ;
	new_prefix->type = AST_CMD_PREFIX;
	new_prefix->u_ast.s_cmd_prefix.io_file = redir;
	new_prefix->u_ast.s_cmd_prefix.cmd_prefix = NULL;
	if (!cmd->u_ast.s_simple_cmd.cmd_prefix)
	{
		cmd->u_ast.s_simple_cmd.cmd_prefix = new_prefix;
		return ;
	}
	prefix = cmd->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix->u_ast.s_cmd_prefix.cmd_prefix)
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	prefix->u_ast.s_cmd_prefix.cmd_prefix = new_prefix;
}

/*
 * helper: attach a single redirection to a command (alias for suffix)
 */
void	attach_redir_to_cmd(t_ast *cmd, t_ast *redir)
{
	attach_redir_suffix(cmd, redir);
}

/*
 * helper: free io_file node
 */
static void	free_io_file(t_ast *io_file)
{
	if (!io_file)
		return ;
	if (io_file->u_ast.s_io_file.op)
		free(io_file->u_ast.s_io_file.op);
	free(io_file);
}

/*
 * helper: free pipe AST with redirections recursively
 */
void	free_pipe_redir_ast_recursive(t_ast *ast)
{
	t_ast	*node;
	t_ast	*next;

	if (!ast)
		return ;
	if (ast->type == AST_PIPE_SEQ)
	{
		free_pipe_redir_ast_recursive(ast->u_ast.s_pipe_seq.left);
		free_pipe_redir_ast_recursive(ast->u_ast.s_pipe_seq.right);
		free(ast);
		return ;
	}
	if (ast->type == AST_SIMPLE_CMD)
	{
		node = ast->u_ast.s_simple_cmd.cmd_prefix;
		while (node)
		{
			next = node->u_ast.s_cmd_prefix.cmd_prefix;
			free_io_file(node->u_ast.s_cmd_prefix.io_file);
			free(node);
			node = next;
		}
		node = ast->u_ast.s_simple_cmd.cmd_suffix;
		while (node)
		{
			next = node->u_ast.s_cmd_suffix.cmd_suffix;
			free_io_file(node->u_ast.s_cmd_suffix.io_file);
			free(node);
			node = next;
		}
	}
	free(ast);
}

/*
 * teardown: free pipe+redir AST
 */
int	teardown_free_pipe_redir_ast(void **state)
{
	if (!state || !*state)
		return (0);
	free_pipe_redir_ast_recursive((t_ast *)(*state));
	*state = NULL;
	return (0);
}
