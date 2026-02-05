/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:59:17 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 14:59:31 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

t_quote_state	update_quote_state(char c, t_quote_state current)
{
	if (current == QUOTE_NONE)
	{
		if (c == '\'')
			return (QUOTE_SINGLE);
		if (c == '"')
			return (QUOTE_DOUBLE);
	}
	else if (current == QUOTE_SINGLE && c == '\'')
		return (QUOTE_NONE);
	else if (current == QUOTE_DOUBLE && c == '"')
		return (QUOTE_NONE);
	return (current);
}

int	is_in_single_quotes(t_quote_state state)
{
	return (state == QUOTE_SINGLE);
}

int	is_in_double_quotes(t_quote_state state)
{
	return (state == QUOTE_DOUBLE);
}

static int	should_remove_quote(char c, t_quote_state state,
	t_quote_state next_state)
{
	if (c == '\'' && (state == QUOTE_NONE || state == QUOTE_SINGLE))
		return (state != next_state);
	if (c == '"' && (state == QUOTE_NONE || state == QUOTE_DOUBLE))
		return (state != next_state);
	return (0);
}

char	*remove_quotes(const char *str)
{
	char			*result;
	t_quote_state	state;
	t_quote_state	next_state;
	int				i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	state = QUOTE_NONE;
	i = 0;
	while (str[i])
	{
		next_state = update_quote_state(str[i], state);
		if (!should_remove_quote(str[i], state, next_state))
			result = ft_charjoin(result, str[i]);
		state = next_state;
		i++;
	}
	return (result);
}
