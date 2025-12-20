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
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

static void assert_token(t_token *token, t_token_type expected_type, 
                        const char *expected_literal)
{
	assert_non_null(token);
	assert_int_equal(token->type, expected_type);
	if (expected_literal)
	{
		assert_non_null(token->literal);
		assert_string_equal(token->literal, expected_literal);
	}
}

static void print_tokens(t_list *tokens)
{
	t_list *current = tokens;
	int i = 0;
	
	printf("\n=== Tokens ===\n");
	while (current)
	{
		t_token *token = (t_token *)current->content;
		printf("[%d] Type: %d, Literal: '%s'\n", i++, token->type, 
		       token->literal ? token->literal : "NULL");
		current = current->next;
	}
	printf("==============\n");
}

// ============================================================================
// BASIC TOKEN TESTS
// ============================================================================

static void test_single_word(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, END, NULL);
	free_token(token);
	
	free_lexer(lexer);
}

static void test_multiple_words(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello world");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "world");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, END, NULL);
	free_token(token);
	
	free_lexer(lexer);
}

static void test_empty_input(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, END, NULL);
	free_token(token);
	
	free_lexer(lexer);
}

static void test_whitespace_only(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("   \t  \n  ");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, END, NULL);
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// OPERATOR TESTS
// ============================================================================

static void test_pipe_operator(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello | grep hello");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "grep");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_redirect_input(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("cat < file.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, LESS, "<");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "file.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_redirect_output(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo test > output.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "test");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_redirect_append(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo test >> output.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "test");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, DGREAT, ">>");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_heredoc(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("cat << EOF");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, DLESS, "<<");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "EOF");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_and_operator(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello && echo world");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, AND_IF, "&&");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "world");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_or_operator(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello || echo world");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, OR_IF, "||");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "world");
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// BONUS: PARENTHESES TESTS
// ============================================================================

static void test_parentheses(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("(echo hello)");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_nested_parentheses(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("((echo hello))");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// QUOTING TESTS
// ============================================================================

static void test_single_quotes(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo 'hello world'");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "'hello world'");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_double_quotes(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo \"hello world\"");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "\"hello world\"");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_quotes_with_metacharacters(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo 'hello | world'");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "'hello | world'");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_mixed_quotes(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo \"hello 'world'\"");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "\"hello 'world'\"");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_empty_quotes(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo ''");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "''");
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// COMPLEX COMMAND TESTS
// ============================================================================

static void test_complex_pipeline(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("cat file.txt | grep test | wc -l");
	assert_non_null(lexer);
	
	t_token *token;
	const char *expected[] = {"cat", "file.txt", "|", "grep", "test", 
	                          "|", "wc", "-l", NULL};
	t_token_type types[] = {WORD, WORD, PIPE, WORD, WORD, PIPE, WORD, WORD};
	
	for (int i = 0; expected[i]; i++)
	{
		token = get_next_token(lexer);
		assert_token(token, types[i], expected[i]);
		free_token(token);
	}
	
	free_lexer(lexer);
}

static void test_multiple_redirects(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("cat < input.txt > output.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, LESS, "<");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "input.txt");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_operators_without_spaces(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello>output.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_subshell_with_pipeline(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("(echo hello | grep h)");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "grep");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "h");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// EDGE CASES
// ============================================================================

static void test_multiple_pipes(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo | | cat");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "cat");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_word_with_numbers(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("file123 test456");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "file123");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "test456");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_special_characters_in_word(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("test_file.txt");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "test_file.txt");
	free_token(token);
	
	free_lexer(lexer);
}

static void test_ampersand_operator(void **state)
{
	(void)state;
	
	t_lexer *lexer = create_lexer("echo hello &");
	assert_non_null(lexer);
	
	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");
	free_token(token);
	
	token = get_next_token(lexer);
	assert_token(token, AMPERSAND, "&");
	free_token(token);
	
	free_lexer(lexer);
}

// ============================================================================
// MAIN TEST SUITE
// ============================================================================

int main(void)
{
	const struct CMUnitTest tests[] = {
		// Basic tests
		cmocka_unit_test(test_single_word),
		cmocka_unit_test(test_multiple_words),
		cmocka_unit_test(test_empty_input),
		cmocka_unit_test(test_whitespace_only),
		
		// Operator tests
		cmocka_unit_test(test_pipe_operator),
		cmocka_unit_test(test_redirect_input),
		cmocka_unit_test(test_redirect_output),
		cmocka_unit_test(test_redirect_append),
		cmocka_unit_test(test_heredoc),
		cmocka_unit_test(test_and_operator),
		cmocka_unit_test(test_or_operator),
		
		// Bonus: Parentheses
		cmocka_unit_test(test_parentheses),
		cmocka_unit_test(test_nested_parentheses),
		
		// Quoting tests
		cmocka_unit_test(test_single_quotes),
		cmocka_unit_test(test_double_quotes),
		cmocka_unit_test(test_quotes_with_metacharacters),
		cmocka_unit_test(test_mixed_quotes),
		cmocka_unit_test(test_empty_quotes),
		
		// Complex commands
		cmocka_unit_test(test_complex_pipeline),
		cmocka_unit_test(test_multiple_redirects),
		cmocka_unit_test(test_operators_without_spaces),
		cmocka_unit_test(test_subshell_with_pipeline),
		
		// Edge cases
		cmocka_unit_test(test_multiple_pipes),
		cmocka_unit_test(test_word_with_numbers),
		cmocka_unit_test(test_special_characters_in_word),
		cmocka_unit_test(test_ampersand_operator),
	};
	
	return cmocka_run_group_tests(tests, NULL, NULL);
}