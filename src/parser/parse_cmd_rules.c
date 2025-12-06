/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_rules.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 07:43:26 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/06 17:20:45 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

t_ast	*parse_subshell(t_parser *parser)
{
	t_ast	and_or;

	and_or = parse_and_or(parser);
	return ;
}

void	parse_io_redirect(t_parser *parser)
{
	return ;
}
