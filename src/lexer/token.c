/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 10:54:34 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/22 15:45:39 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

void	set_operator(t_hash_item operator, char ch, t_token *token, int idx)
{
	const t_hash_item	dup_operator[] = {{"||", OR_IF}, {"&&", AND_IF},
	{"<<", DLESS}, {">>", DGREAT}};

	if (ch == *(operator.key))
	{
		token->type = dup_operator[idx].value;
		token->literal = dup_operator[idx].key;
		token->len = DUP_SIZE;
	}
	else
	{
		token->type = operator.value;
		token->literal = operator.key;
		token->len = CHAR_SIZE;
	}
}

void	check_duplicate_operators(t_lexer *lexer, t_token *token)
{
	int					i;
	int					size;
	const t_hash_item	operators[] = {{"|", PIPE}, {"&", AMPERSAND},
	{"<", LESS}, {">", GREAT}};

	i = 0;
	size = sizeof(operators) / sizeof(t_hash_item);
	while (i < size)
	{
		if (lexer->ch == *(operators[i].key))
		{
			set_operator(operators[i], peek_char(lexer), token, i);
			if (token->len == DUP_SIZE)
				next_char(lexer);
		}
		i++;
	}
}

void	check_operators(t_lexer *lexer, t_token *token)
{
	int					i;
	int					size;
	const t_hash_item	operators[] = {{"", END}, {"=", EQUAL},
	{"(", LPAREN}, {")", RPAREN}};

	check_duplicate_operators(lexer, token);
	if (token->literal == NULL)
	{
		i = 0;
		size = sizeof(operators) / sizeof(t_hash_item);
		while (i < size)
		{
			if (lexer->ch == *(operators[i].key))
			{
				token->type = operators[i].value;
				token->literal = operators[i].key;
				token->len = CHAR_SIZE;
			}
			i++;
		}
	}
}

t_token	*create_token(t_token token)
{
	t_token	*new_token;

	new_token = ft_calloc(sizeof(t_token), 1);
	if (new_token == NULL)
		return (NULL);
	new_token->type = token.type;
	new_token->literal = ft_substr(token.literal, 0, token.len);
	new_token->len = token.len;
	return (new_token);
}
