/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:52:40 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/23 18:17:21 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "test_eval.h"
#include <stdlib.h>
#include <string.h>

/*
 *   helper: create a left-associative pipeline AST from an array of
 *           command ASTs
 */
t_ast	*create_pipeline_ast(t_ast **cmds, int count)
{
	t_ast	*root;
	t_ast	*node;
	int		i;
	t_ast	*pipe_node;

	if (count == 0)
		return (NULL);
	if (count == 1)
		return (cmds[0]);
	root = NULL;
	node = NULL;
	i = 0;
	while (i < count - 1)
	{
		pipe_node = calloc(1, sizeof(t_ast));
		if (!pipe_node)
			return (NULL);
		pipe_node->type = AST_PIPE_SEQ;
		if (i == 0)
			pipe_node->u_ast.s_pipe_seq.left = cmds[0];
		else
			pipe_node->u_ast.s_pipe_seq.left = node;
		pipe_node->u_ast.s_pipe_seq.right = cmds[i + 1];
		node = pipe_node;
		root = node;
		i++;
	}
	return (root);
}

/*
 *   teardown: free pipeline AST
 */
int	teardown_free_pipe_ast(void **state)
{
	t_ast	*ast;

	if (!state || !*state)
		return (0);
	ast = (t_ast *)(*state);
	free_ast(ast);
	*state = NULL;
	return (0);
}
