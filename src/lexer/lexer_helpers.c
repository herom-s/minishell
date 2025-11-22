/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:09:38 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/22 10:48:28 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

char	peek_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int) ft_strlen(lexer->input))
		return ('\0');
	else
		return (lexer->input[lexer->read_position]);
}

void	next_char(t_lexer *lexer)
{
	lexer->ch = peek_char(lexer);
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_whitespace(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		next_char(lexer);
}
