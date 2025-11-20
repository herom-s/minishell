/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:43:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/20 15:08:57 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "lexer.h"
#include "libft.h"

t_token	*create_identifier(t_lexer *lexer)
{
	t_token	*token;
	int		start;

	start = lexer->position;
	while (ft_isalnum(lexer->ch) || lexer->ch == '_')
		read_char(lexer);
	token = ft_calloc(sizeof(t_token), 1);
	if (token == NULL)
		return (NULL);
	token->type = IDENT;
	token->literal = ft_substr(lexer->input, start, lexer->position - start);
	return (token);
}

t_token	*create_token(t_token_type type, char *ch)
{
	t_token	*token;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->literal = ft_substr(ch, 0, 1);
	return (token);
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
	return lexer;
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;
	
	if (lexer == NULL)
		return (NULL);
	token = NULL;
	skip_whitespace(lexer);
	if (lexer->ch == '=')
		token = create_token(EQUAL, &lexer->ch);
	else if (lexer->ch == '|')
		token = create_token(PIPE, &lexer->ch);
	else if (lexer->ch == '&')
		token = create_token(AMPERSAND, &lexer->ch);
	else if (lexer->ch == '<')
		token = create_token(REDIRECT_IN, &lexer->ch);
	else if (lexer->ch == '>')
		token = create_token(REDIRECT_OUT, &lexer->ch);
	else if (lexer->ch == '*')
		token = create_token(ASTERISK, &lexer->ch);
	else if (lexer->ch == '$')
		token = create_token(DOLLAR, &lexer->ch);
	else if (lexer->ch == '?')
		token = create_token(QUESTION, &lexer->ch);
	else if (lexer->ch == '\'')
		token = create_token(SQUOTE, &lexer->ch);
	else if (lexer->ch == '\"')
		token = create_token(DQUOTE, &lexer->ch);
	else if (lexer->ch == '{')
		token = create_token(LBRACE, &lexer->ch);
	else if (lexer->ch == '}')
		token = create_token(RBRACE, &lexer->ch);
	else if (lexer->ch == '(')
		token = create_token(LPAREN, &lexer->ch);
	else if (lexer->ch == ')')
		token = create_token(RPAREN, &lexer->ch);
	else if (lexer->ch == 0)
		token = create_token(END, 0);
	else
	{
		if (ft_isalpha(lexer->ch))
		{
			token = create_identifier(lexer);
			return (token);
		}
		else
			token = create_token(ILLEGAL, &lexer->ch);
	}
	read_char(lexer);
	return (token);
}
