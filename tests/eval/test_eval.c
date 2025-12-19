/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 12:08:12 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 12:15:42 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
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
	simple->type = SIMPLE_CMD;
	simple->s_simple_cmd.cmd_prefix = NULL;
	simple->s_simple_cmd.cmd_name = strdup(cmd_name);
	simple->s_simple_cmd.cmd_suffix = NULL;
	prev_suffix = NULL;
	i = 0;
	while (i < arg_count)
	{
		suffix = calloc(1, sizeof(t_ast));
		if (!suffix)
			return (NULL);
		suffix->type = CMD_SUFFIX;
		suffix->s_cmd_suffix.io_file = NULL;
		suffix->s_cmd_suffix.word = strdup(args[i]);
		suffix->s_cmd_suffix.cmd_suffix = NULL;
		if (prev_suffix)
			prev_suffix->s_cmd_suffix.cmd_suffix = suffix;
		else
			simple->s_simple_cmd.cmd_suffix = suffix;
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
	if (ast->type == SIMPLE_CMD)
	{
		if (ast->s_simple_cmd.cmd_name)
			free(ast->s_simple_cmd.cmd_name);
		suf = ast->s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->s_cmd_suffix.cmd_suffix;
			if (suf->s_cmd_suffix.word)
				free((void *)suf->s_cmd_suffix.word);
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
	if (ast->type == SIMPLE_CMD)
	{
		if (ast->s_simple_cmd.cmd_name)
			free(ast->s_simple_cmd.cmd_name);
		suf = ast->s_simple_cmd.cmd_suffix;
		while (suf)
		{
			next = suf->s_cmd_suffix.cmd_suffix;
			if (suf->s_cmd_suffix.word)
				free((void *)suf->s_cmd_suffix.word);
			free(suf);
			suf = next;
		}
	}
	free(ast);
	*state = NULL;
	return (0);
}
