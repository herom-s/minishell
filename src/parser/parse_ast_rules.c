/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:40:57 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/05 07:52:23 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

void	parse_list()
{
	return ;
}

t_ast	*parse_and_or(t_parser *parser)
{
	t_ast	node;
	t_ast	*left;
	t_ast	*right;
	t_token	*op;

	left = parse_pipe_sequence(parser);
	if (!cur_token_is(parser->cur_token, (1 << AND_IF) | (1 << OR_IF)))
		return (left);
	op = parser->cur_token;
	next_token(parser);
	if (cur_token_is(parser->cur_token, (1 << AND_IF) | (1 << OR_IF)
			| (1 << PIPE)))
	{
		parse_error(parser);
		return (NULL);
	}
	right = parse_and_or(parser);
	node = (t_ast){.type = AST_AND_OR, .left = left, .op = op,
		.right = right};
	return (create_ast(parent));
}

void	parse_pipe_sequence()
{
	return ;
}

void	parse_io_redirect()
{
	return ;
}
