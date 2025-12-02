/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 06:50:01 by marvin            #+#    #+#             */
/*   Updated: 2025/12/02 07:09:34 by marvin           ###   ########.fr       */
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
	const char	ast_type_hashmap[] = {
	[LIST] = "list",
	[AND_OR] = "and_or",
	[PIPE_SEQ] = "pipe_seq",
	[SUBSHELL] = "subshell",
	[SIMPLE_CMD] = "simple_cmd",
	[CMD_PREFIX] = "cmd_prefix",
	[CMD_SUFFIX] = "cmd_suffix",
	[IO_FILE] = "io_file"};

	return (ast_type_hashmap[type]);
}
