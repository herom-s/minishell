/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:17:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/01 14:17:48 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void	test_one_char_operators(void **state)
{
	const t_hash_item	operators[] = {{"|", PIPE}, {"&", AMPERSAND}, {"<",
			LESS}, {">", GREAT}, {"\0", END}};
	int					i;
	char				*str;
	t_lexer				*lexer;
	t_token				*token;

	(void)state;
	i = 0;
	str = "|&<>";
	lexer = create_lexer(str);
	while (str[i])
	{
		token = get_next_token(lexer);
		assert_true(token->type == operators[i].value);
		assert_true(ft_strncmp(token->literal, operators[i].key,
				token->len) == 0);
		free(token->literal);
		free(token);
		i++;
	}
	free(lexer);
}
