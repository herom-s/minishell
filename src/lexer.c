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

void	skip_whitespace(t_lexer *lexer)
{
	while (ft_isspace(lexer->ch)) {
		read_char(lexer);
	}
}

t_token	*create_token(t_token_type type, char ch)
{
	t_token	*token;

	token = (token *)ft_calloc(sizeof(token), 1);
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->literal = &ch;
	return (token);
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;
	
	skip_whitespace(lexer);
	if (lexer->ch == '=')
		token = create_token(T_ASSIGN, lexer->ch);
	else if (lexer->ch == '|')
		token = create_token(T_PIPE, lexer->ch);
	else if (lexer->ch == '&')
		token = create_token(T_AMPERSAND, lexer->ch);
	else if (lexer->ch == '<')
		token = create_token(T_REDIRECT_IN, lexer->ch);
	else if (lexer->ch == '>')
		token = create_token(T_REDIRECT_OUT, lexer->ch);
	else if (lexer->ch == '*')
		token = create_token(T_ASTERISK, lexer->ch);
	else if (lexer->ch == '$')
		token = create_token(T_DOLLAR, lexer->ch);
	else if (lexer->ch == '?')
		token = create_token(T_QUESTION, lexer->ch);
	else if (lexer->ch == '\'')
		token = create_token(T_SQUOTE, lexer->ch);
	else if (lexer->ch == '\"')
		token = create_token(T_DQUOTE, lexer->ch);
	else if (lexer->ch == '{')
		token = create_token(T_LBRACE, lexer->ch);
	else if (lexer->ch == '}')
		token = create_token(T_RBRACE, lexer->ch);
	else if (lexer->ch == '(')
		token = create_token(T_LPAREN, lexer->ch);
	else if (lexer->ch == ')')
		token = create_token(T_RPAREN, lexer->ch);
	else if (lexer->ch == 0)
		token = create_token(T_EOF, '');
	else
	{
		if (ft_isalpha(lexer->ch))
			// look for indetifier
		else
			token = create_token(T_ILLEGAL, lexer->ch);
	}
	return (token);
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

	lexer = (lexer *)ft_calloc(sizeof(lexer), 1);
	if (lexer == NULL)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->ch = input[lexer->position];
	return lexer;
}
