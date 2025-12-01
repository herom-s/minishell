/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 05:44:00 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/01 07:32:59 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

int	cur_token_is(t_token_type type, t_token_type expected)
{
	return (type == expected);
}

int	peek_token_is(t_token_type type, t_token_type expected)
{
	return (type == expected);
}
