/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:43:21 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/27 20:42:36 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "lexer.h"
#include "libft.h"

int	check_backslash(t_lexer *lexer)
{
	if (lexer->ch == '\\')
	{
		next_char(lexer);
		if (lexer->ch != '\0')
			next_char(lexer);
		return (1);
	}
	return (0);
}

void	read_word(t_lexer *lexer, t_token *token)
{
	int		start;
	char	quote;

	quote = '\0';
	start = lexer->position;
	while (lexer->ch != '\0' && (!is_metacharacter(lexer->ch)
				|| quote != '\0'))
	{
		if (check_backslash(lexer) == 1)
			continue ;
		if (is_quoting(lexer->ch))
		{
			if (quote == '\0')
				quote = lexer->ch;
			else if (quote == lexer->ch)
				quote = '\0';
		}
		next_char(lexer);
	}
	*token = (t_token) {WORD, lexer->input + start, lexer->position - start};
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

t_lexer	*create_lexer(char *input)
{
	t_lexer	*lexer;

	if (input == NULL)
		return (NULL);
	lexer = ft_calloc(sizeof(t_lexer), 1);
	if (lexer == NULL)
		return (NULL);
	lexer->input = input;
	lexer->input_len = ft_strlen(input);
	lexer->position = 0;
	lexer->read_position = 1;
	lexer->ch = lexer->input[lexer->position];
	return (lexer);
}

void	free_lexer(t_lexer *lexer)
{
	free(lexer);
}
