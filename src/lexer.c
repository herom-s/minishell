/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thawan <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 07:32:40 by thawan            #+#    #+#             */
/*   Updated: 2025/11/19 07:41:38 by thawan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token_type	get_next_token(t_lexer *lexer)
{
	t_token_type	type;
	if (lexer->ch == '=')
		type = create_token(T_ASSIGN, lexer->ch);
	else if (lexer->ch == '|')
		type = create_token(T_PIPE, lexer->ch);
	else if (lexer->ch == '&')
		type = create_token(T_AMPERSAND, lexer->ch);
	else if (lexer->ch == '<')
		type = create_token(T_REDIRECT_IN, lexer->ch);
	else if (lexer->ch == '>')
		type = create_token(T_REDIRECT_OUT, lexer->ch);
	else if (lexer->ch == '*')
		type = create_token(T_ASTERISK, lexer->ch);
	else if (lexer->ch == '$')
		type = create_token(T_DOLLAR, lexer->ch);
	else if (lexer->ch == '?')
		type = create_token(T_QUESTION, lexer->ch);
	else if (lexer->ch == 0)
		type = create_token(T_EOF, lexer->ch);
	// Create other tokens
}

void	read_char(t_lexer *lexer)
{
	if (lexer->next_position >= strlen(lexer->input))
		lexer->ch = 0;
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

t_lexer	*create_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = (lexer *)malloc(sizeof(lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->ch = input[lexer->position];
	return lexer;
}
