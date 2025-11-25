#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "lexer.h"
#include <unistd.h>

/* 
 * Tests for minishell's main function
 * Since the main just prints "Hello, 42!\n", we'll test:
 * 1. Basic functionality
 * 2. Output validation (if we redirect stdout)
 * 3. Return value
 * 4. Argument handling
 */

static void test_one_char_operators(void **state)
{
	(void)state;
	const t_hash_item	operators[] = {{"|", PIPE}, {"&", AMPERSAND},
		{"<", LESS}, {">", GREAT}, {"\0", END}};
	int	i = 0;
	char	*str = "|&<>";
	t_lexer	*lexer = create_lexer(str);
	t_token	*token;
	while (str[i]) {
		token = get_next_token(lexer);
		assert_true(token->type == operators[i].value);
		assert_true(ft_strncmp(token->literal, operators[i].key, token->len) == 0);
		free(token->literal);
		free(token);
		i++;
	}
	free(lexer);
}

int main(void)
{
    printf("\n=== Running Minishell Tests ===\n");
    printf("Testing lexer);\n\n");
    
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_one_char_operators),
	};

    int result = cmocka_run_group_tests(tests, NULL, NULL);
    
    printf("\n=== Test Suite Complete ===\n");
    printf("All tests verify the lexer structure and behavior.\n\n");
    
    return result;
}
