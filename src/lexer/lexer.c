/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:43:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/22 15:18:34 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "lexer.h"
#include "libft.h"

void	read_word(t_lexer *lexer, t_token *token)
{
	int		start;

	start = lexer->position;
	while (ft_isalnum(lexer->ch) || lexer->ch == '_' || lexer->ch == '-')
		next_char(lexer);
	token->type = WORD;
	token->literal = &(lexer->input[start]);
	token->len = lexer->position - start;
}

t_lexer	*create_lexer(char *input)
{
	t_lexer	*lexer;

	if (input == NULL)
		return (NULL);
	lexer = ft_calloc(sizeof(t_lexer), 1);
	if (lexer == NULL)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->read_position = 1;
	lexer->ch = lexer->input[lexer->position];
	return (lexer);
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	token;
	t_token	*new_token;

	if (lexer == NULL)
		return (NULL);
	token.type = -1;
	token.literal = NULL;
	token.len = -1;
	skip_whitespace(lexer);
	check_operators(lexer, &token);
	if (token.literal == NULL)
	{
		read_word(lexer, &token);
		return (create_token(token));
	}
	next_char(lexer);
	new_token = create_token(token);
	return (new_token);
}
