/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:04:12 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/22 10:19:20 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_PARSER_H
# define TEST_PARSER_H

// Simple command tests
void test_parser_simple_command_single_word(void **state);
void test_parser_simple_command_with_args(void **state);
void test_parser_simple_command_with_path(void **state);
void test_parser_command_multiple_args(void **state);

// Pipeline tests
void test_parser_simple_pipe(void **state);
void test_parser_multiple_pipes(void **state);

// Redirection tests
void test_parser_output_redirect(void **state);
void test_parser_input_redirect(void **state);
void test_parser_append_redirect(void **state);
void test_parser_heredoc(void **state);
void test_parser_multiple_redirects(void **state);

// Logical operator tests
void test_parser_and_operator(void **state);
void test_parser_or_operator(void **state);
void test_parser_mixed_logical_operators(void **state);
void test_parser_precedence_and_vs_pipe(void **state);

// Subshell tests (bonus)
void test_parser_simple_subshell(void **state);
void test_parser_subshell_with_pipe(void **state);
void test_parser_nested_subshells(void **state);
void test_parser_subshell_with_logical_ops(void **state);

// Complex combination tests
void test_parser_pipe_with_redirects(void **state);
void test_parser_logical_with_pipes(void **state);
void test_parser_subshell_in_pipeline(void **state);
void test_parser_subshell_and_logical(void **state);

// Edge cases and error handling
void test_parser_empty_input(void **state);
void test_parser_only_pipe(void **state);
void test_parser_pipe_without_right_side(void **state);
void test_parser_redirect_without_filename(void **state);
void test_parser_unclosed_subshell(void **state);
void test_parser_unopened_subshell(void **state);
void test_parser_double_pipe(void **state);
void test_parser_double_redirect(void **state);

// Quote handling tests
void test_parser_command_with_quoted_args(void **state);
void test_parser_redirect_with_quoted_filename(void **state);
void test_parser_mixed_quotes(void **state);

#endif
