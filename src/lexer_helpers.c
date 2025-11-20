/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:09:38 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/20 15:09:44 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

void	read_char(t_lexer *lexer)
{
	if (lexer->read_position >= (int) ft_strlen(lexer->input))
		lexer->ch = 0;
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

void	skip_whitespace(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch))
		read_char(lexer);
}
