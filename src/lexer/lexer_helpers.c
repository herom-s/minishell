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

int	is_metacharacter(char ch)
{
	return (ch == '|' || ch == '&' || ch == '(' || ch == ')' || ch == '<'
		|| ch == '>' || ch == ' ' || ch == '\t' || ch == '\n' || ch == ';');
}

int	is_quoting(char ch)
{
	return (ch == '\'' || ch == '\"');
}

char	peek_char(t_lexer *lexer)
{
	if (lexer->read_position >= lexer->input_len)
		return ('\0');
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
