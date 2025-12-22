/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:17:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/22 09:17:39 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void assert_token(t_token *token, t_token_type expected_type,
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

void print_tokens(t_list *tokens)
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

void test_single_word(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, END, NULL);

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_multiple_words(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello world");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, WORD, "world");

	token = get_next_token(lexer);
	assert_token(token, END, NULL);

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_empty_input(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, END, NULL);

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_whitespace_only(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("   \t  \n  ");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, END, NULL);

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

// ============================================================================
// OPERATOR TESTS
// ============================================================================

void test_pipe_operator(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello | grep hello");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");

	token = get_next_token(lexer);
	assert_token(token, WORD, "grep");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_redirect_input(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("cat < file.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");

	token = get_next_token(lexer);
	assert_token(token, LESS, "<");

	token = get_next_token(lexer);
	assert_token(token, WORD, "file.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_redirect_output(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo test > output.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "test");

	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");

	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_redirect_append(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo test >> output.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "test");

	token = get_next_token(lexer);
	assert_token(token, DGREAT, ">>");

	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_heredoc(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("cat << EOF");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");

	token = get_next_token(lexer);
	assert_token(token, DLESS, "<<");

	token = get_next_token(lexer);
	assert_token(token, WORD, "EOF");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_and_operator(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello && echo world");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, AND_IF, "&&");

	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "world");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_or_operator(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello || echo world");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, OR_IF, "||");

	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "world");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

// ============================================================================
// BONUS: PARENTHESES TESTS
// ============================================================================

void test_parentheses(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("(echo hello)");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");

	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_nested_parentheses(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("((echo hello))");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");

	token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");

	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");

	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

// ============================================================================
// QUOTING TESTS
// ============================================================================

void test_single_quotes(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo 'hello world'");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "'hello world'");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_double_quotes(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo \"hello world\"");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "\"hello world\"");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_quotes_with_metacharacters(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo 'hello | world'");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "'hello | world'");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_mixed_quotes(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo \"hello 'world'\"");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "\"hello 'world'\"");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_empty_quotes(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo ''");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "''");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

// ============================================================================
// COMPLEX COMMAND TESTS
// ============================================================================

void test_complex_pipeline(void **state)
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
	}

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_multiple_redirects(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("cat < input.txt > output.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "cat");

	token = get_next_token(lexer);
	assert_token(token, LESS, "<");

	token = get_next_token(lexer);
	assert_token(token, WORD, "input.txt");

	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");

	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_operators_without_spaces(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello>output.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, GREAT, ">");

	token = get_next_token(lexer);
	assert_token(token, WORD, "output.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_subshell_with_pipeline(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("(echo hello | grep h)");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, LPAREN, "(");

	token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");

	token = get_next_token(lexer);
	assert_token(token, WORD, "grep");

	token = get_next_token(lexer);
	assert_token(token, WORD, "h");

	token = get_next_token(lexer);
	assert_token(token, RPAREN, ")");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

// ============================================================================
// EDGE CASES
// ============================================================================

void test_multiple_pipes(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo | | cat");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");

	token = get_next_token(lexer);
	assert_token(token, PIPE, "|");

	token = get_next_token(lexer);
	assert_token(token, WORD, "cat");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_word_with_numbers(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("file123 test456");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "file123");

	token = get_next_token(lexer);
	assert_token(token, WORD, "test456");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_special_characters_in_word(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("test_file.txt");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "test_file.txt");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

void test_ampersand_operator(void **state)
{
	(void)state;

	t_lexer *lexer = create_lexer("echo hello &");
	assert_non_null(lexer);

	t_token *token = get_next_token(lexer);
	assert_token(token, WORD, "echo");

	token = get_next_token(lexer);
	assert_token(token, WORD, "hello");

	token = get_next_token(lexer);
	assert_token(token, AMPERSAND, "&");

	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}
