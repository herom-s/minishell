/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 12:08:12 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/23 18:16:41 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "test_eval.h"
#include <stdlib.h>
#include <string.h>

/*
 *   create: creates cmd AST from parameters
 */

t_ast	*create_cmd_ast(char *cmd_name, char **args, int arg_count)
{
	t_ast	*simple;
	t_ast	*suffix;
	t_ast	*prev_suffix;
	int		i;

	simple = calloc(1, sizeof(t_ast));
	if (!simple)
		return (NULL);
	simple->type = AST_SIMPLE_CMD;
	simple->u_ast.s_simple_cmd.cmd_prefix = NULL;
	simple->u_ast.s_simple_cmd.cmd_name = cmd_name;
	simple->u_ast.s_simple_cmd.cmd_suffix = NULL;
	prev_suffix = NULL;
	i = 0;
	while (i < arg_count)
	{
		suffix = calloc(1, sizeof(t_ast));
		if (!suffix)
			return (NULL);
		suffix->type = AST_CMD_SUFFIX;
		suffix->u_ast.s_cmd_suffix.io_file = NULL;
		suffix->u_ast.s_cmd_suffix.word = args[i];
		suffix->u_ast.s_cmd_suffix.cmd_suffix = NULL;
		if (prev_suffix)
			prev_suffix->u_ast.s_cmd_suffix.cmd_suffix = suffix;
		else
			simple->u_ast.s_simple_cmd.cmd_suffix = suffix;
		prev_suffix = suffix;
		i++;
	}
	return (simple);
}

void	free_cmd_ast(t_ast *ast)
{
	t_ast	*suf;
	t_ast	*next;

	if (!ast)
		return ;
	if (ast->type == AST_SIMPLE_CMD)
	{
		suf = ast->u_ast.s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->u_ast.s_cmd_suffix.cmd_suffix;
			free(suf);
			suf = next;
		}
	}
	free(ast);
}

/*
 *  teardown: free cmd AST with chained suffixes
 */
int	teardown_free_cmd_ast(void **state)
{
	t_ast	*ast;
	t_ast	*suf;
	t_ast	*next;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	if (ast->type == AST_SIMPLE_CMD)
	{
		suf = ast->u_ast.s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->u_ast.s_cmd_suffix.cmd_suffix;
			free(suf);
			suf = next;
		}
	}
	free(ast);
	*state = NULL;
	return (0);
}
