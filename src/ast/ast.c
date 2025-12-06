/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 06:54:33 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/06 10:49:42 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "ast.h"
#include "parser.h"
#include "libft.h"

// Do later
// do a if-else check for the type of node and print
void	print_ast(t_ast *node)
{
	if (!node)
		return ;
}

char	*ast_type_to_str(t_ast_type type)
{
	const char	*ast_literal_list[] = {"list", "and_or", "pipe_seq",
		"subshell", "simple_cmd", "cmd_prefix", "cmd_suffix", "io_file"};

	return ((char *)ast_literal_list[type]);
}

t_ast	*create_node(t_ast node)
{
	t_ast	*ast_node;

	ast_node = ft_calloc(1, sizeof(t_ast));
	if (ast_node == NULL)
		return (NULL);
	*ast_node = node;
	return (ast_node);
}
