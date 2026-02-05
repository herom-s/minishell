/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 08:41:55 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/22 08:41:58 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_LEXER_H
# define TEST_LEXER_H

// Basic token tests
void test_single_word(void **state);
void test_multiple_words(void **state);
void test_empty_input(void **state);
void test_whitespace_only(void **state);

// Operator tests
void test_pipe_operator(void **state);
void test_redirect_input(void **state);
void test_redirect_output(void **state);
void test_redirect_append(void **state);
void test_heredoc(void **state);
void test_and_operator(void **state);
void test_or_operator(void **state);

// Bonus: Parentheses tests
void test_parentheses(void **state);
void test_nested_parentheses(void **state);

// Quoting tests
void test_single_quotes(void **state);
void test_double_quotes(void **state);
void test_quotes_with_metacharacters(void **state);
void test_mixed_quotes(void **state);
void test_empty_quotes(void **state);

// Complex command tests
void test_complex_pipeline(void **state);
void test_multiple_redirects(void **state);
void test_operators_without_spaces(void **state);
void test_subshell_with_pipeline(void **state);

// Edge cases
void test_multiple_pipes(void **state);
void test_word_with_numbers(void **state);
void test_special_characters_in_word(void **state);
void test_ampersand_operator(void **state);

#endif
