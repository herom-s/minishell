/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 21:28:04 by marvin            #+#    #+#             */
/*   Updated: 2025/11/27 21:28:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "lexer.h"

void test_one_char_operators(void **state)
{
    (void)state;
	const t_hash_item operators[] = {{"|", PIPE}, {"&", AMPERSAND}, {"<", LESS}, {">", GREAT}, {"\0", END}};
    int i = 0;
    char *str = "|&<>";
    t_lexer *lexer = create_lexer(str);
    t_token *token;

    while (str[i])
    {
        token = get_next_token(lexer);
        assert_true(token->type == operators[i].value);
        assert_true(ft_strncmp(token->literal, operators[i].key, token->len) == 0);
        free(token->literal);
        free(token);
        i++;
    }

    free(lexer);
}
