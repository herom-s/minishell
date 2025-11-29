/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:13:51 by thaperei          #+#    #+#             */
/*   Updated: 2025/11/28 22:13:54 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

void	set_operators(t_hash_item operator, char ch, t_token *token,
		int idx)
{
	const t_hash_item	dup_operators[] = {{"||", OR_IF}, {"&&", AND_IF},
	{"<<", DLESS}, {">>", DGREAT}};

	if (ch == *(operator.key))
	{
		*token = (t_token){dup_operators[idx].value, dup_operators[idx].key,
			DUP_SIZE};
	}
	else
		*token = (t_token){operator.value, operator.key, CHAR_SIZE};
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
			set_operators(operators[i], peek_char(lexer), token, i);
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
				*token = (t_token){operators[i].value, operators[i].key,
					CHAR_SIZE};
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
