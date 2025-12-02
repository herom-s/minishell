/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 06:50:01 by marvin            #+#    #+#             */
/*   Updated: 2025/12/02 18:58:04 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "ast.h"
#include "parser.h"

// Do later
// do a if-else check for the type of node and print
void	print_ast(t_ast *node)
{
	if (!node)
		return ;
}

char	*ast_type_to_str(t_ast_type type)
{
	const char	*ast_literal_list[] = { "list", "and_or", "pipe_seq",
		"subshell", "simple_cmd", "cmd_prefix", "cmd_suffix", "io_file"};

	return ((char *) ast_literal_list[type]);
}
