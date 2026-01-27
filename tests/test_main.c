/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:14:46 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 17:53:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_eval.h"
#include "test_lexer.h"
#include "test_parser.h"
#include "test_signal.h"
#include "tests.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

static int	run_signal_tests(void)
{
	const struct CMUnitTest tests[] = {
		// Signal setup tests
		cmocka_unit_test_setup_teardown(test_setup_signal_success,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_setup_signal_configures_sigint,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_setup_signal_ignores_sigquit,
			setup_signal_test, teardown_signal_test),

		// SIGINT handler tests
		cmocka_unit_test_setup_teardown(test_sigint_sets_global_flag,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_sigint_multiple_times,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_sigint_does_not_terminate_process,
			setup_signal_test, teardown_signal_test),

		// SIGQUIT handler tests
		cmocka_unit_test_setup_teardown(test_sigquit_is_ignored,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_sigquit_multiple_times_ignored,
			setup_signal_test, teardown_signal_test),

		// Child process tests
		cmocka_unit_test_setup_teardown(test_sigint_in_child_process,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_child_inherits_signal_handlers,
			setup_signal_test, teardown_signal_test),

		// Signal restoration tests
		cmocka_unit_test_setup_teardown(test_signal_can_be_restored_to_default,
			setup_signal_test, teardown_signal_test),

		// Flag reset tests
		cmocka_unit_test_setup_teardown(test_global_flag_can_be_reset,
			setup_signal_test, teardown_signal_test),

		// Concurrent signal tests
		cmocka_unit_test_setup_teardown(test_sigint_and_sigquit_together,
			setup_signal_test, teardown_signal_test),

		// Edge case tests
		cmocka_unit_test_setup_teardown(test_setup_signal_called_twice,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_sigint_with_zero_flag,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_rapid_signal_delivery,
			setup_signal_test, teardown_signal_test),

		// Sigaction tests
		cmocka_unit_test_setup_teardown(test_sigaction_flags_are_correct,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_signal_mask_is_empty,
			setup_signal_test, teardown_signal_test),

		// Integration tests
		cmocka_unit_test_setup_teardown(test_signal_handler_survives_child_exit,
			setup_signal_test, teardown_signal_test),
		cmocka_unit_test_setup_teardown(test_multiple_children_with_signals,
			setup_signal_test, teardown_signal_test),
	};
	printf("\n--- signal Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_parser_tests(void)
{
	const struct CMUnitTest tests[] = {
		// Simple command tests
		cmocka_unit_test(test_parser_simple_command_single_word),
		cmocka_unit_test(test_parser_simple_command_with_args),
		cmocka_unit_test(test_parser_simple_command_with_path),
		cmocka_unit_test(test_parser_command_multiple_args),
		// Pipeline tests
		cmocka_unit_test(test_parser_simple_pipe),
		cmocka_unit_test(test_parser_multiple_pipes),
		// Redirection tests
		cmocka_unit_test(test_parser_output_redirect),
		cmocka_unit_test(test_parser_input_redirect),
		cmocka_unit_test(test_parser_append_redirect),
		cmocka_unit_test(test_parser_heredoc),
		cmocka_unit_test(test_parser_multiple_redirects),
		// Logical operator tests
		cmocka_unit_test(test_parser_and_operator),
		cmocka_unit_test(test_parser_or_operator),
		cmocka_unit_test(test_parser_mixed_logical_operators),
		cmocka_unit_test(test_parser_precedence_and_vs_pipe),
		// Subshell tests (bonus)
		cmocka_unit_test(test_parser_simple_subshell),
		cmocka_unit_test(test_parser_subshell_with_pipe),
		cmocka_unit_test(test_parser_nested_subshells),
		cmocka_unit_test(test_parser_subshell_with_logical_ops),
		// Complex combinations
		cmocka_unit_test(test_parser_pipe_with_redirects),
		cmocka_unit_test(test_parser_logical_with_pipes),
		cmocka_unit_test(test_parser_subshell_in_pipeline),
		cmocka_unit_test(test_parser_subshell_and_logical),
		// Edge cases and errors
		cmocka_unit_test(test_parser_empty_input),
		cmocka_unit_test(test_parser_only_pipe),
		cmocka_unit_test(test_parser_pipe_without_right_side),
		cmocka_unit_test(test_parser_redirect_without_filename),
		cmocka_unit_test(test_parser_unclosed_subshell),
		cmocka_unit_test(test_parser_unopened_subshell),
		cmocka_unit_test(test_parser_double_pipe),
		cmocka_unit_test(test_parser_double_redirect),
		// Quote handling
		cmocka_unit_test(test_parser_command_with_quoted_args),
		cmocka_unit_test(test_parser_redirect_with_quoted_filename),
		cmocka_unit_test(test_parser_mixed_quotes),
	};
	printf("\n--- parser Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_lexer_tests(void)
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
	printf("\n--- lexer Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_echo_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_basic,
			setup_built_in_echo_basic_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_n_basic,
			setup_built_in_echo_n_basic_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_empty,
			setup_built_in_echo_empty_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_n_empty,
			setup_built_in_echo_n_empty_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_mutiple_args,
			setup_built_in_echo_mutiple_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_echo_n_mutiple_args,
			setup_built_in_echo_n_mutiple_args_ast, teardown_free_cmd_ast),
	};
	printf("\n--- echo Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_pwd_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_pwd_basic,
			setup_built_in_pwd_basic_ast, teardown_free_cmd_ast),
	};
	printf("\n--- pwd Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_cd_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_cd_path,
			setup_built_in_cd_path_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_cd_to_many_args,
			setup_built_in_cd_to_many_args_ast, teardown_free_cmd_ast),
	};
	printf("\n--- cd Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_env_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_env_no_args,
			setup_built_in_env_no_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_env_not_empty,
			setup_built_in_env_no_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_env_contains_path,
			setup_built_in_env_no_args_ast, teardown_free_cmd_ast),
	};
	printf("\n--- env Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_exit_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_exit_single_arg_path,
			setup_built_in_exit_single_arg_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_exit_to_many_args_path,
			setup_built_in_exit_to_many_args_ast, teardown_free_cmd_ast),
	};
	printf("\n--- exit Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_export_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_no_args,
			setup_built_in_export_no_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_arg,
			setup_built_in_export_arg_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_mutiple_args,
			setup_built_in_export_mutiple_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_invalid_digit,
			setup_built_in_export_invalid_digit_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_invalid_special,
			setup_built_in_export_invalid_special_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_export_mixed_args,
			setup_built_in_export_mixed_args_ast, teardown_free_cmd_ast),
	};
	printf("\n--- export Builtin Tests ---\n");
	return (cmocka_run_group_tests_name("export", tests, NULL, NULL));
}

static int	run_unset_builtin_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_built_in_unset_no_args,
			setup_built_in_unset_no_args_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_unset_single_var,
			setup_built_in_unset_single_var_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_unset_multiple_vars,
			setup_built_in_unset_multiple_vars_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_built_in_unset_nonexistent,
			setup_built_in_unset_nonexistent_ast, teardown_free_cmd_ast),
	};
	printf("\n--- unset Builtin Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_external_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_external_cat_abs,
			setup_external_cat_abs_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_external_invalid_abs,
			setup_external_invalid_abs_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_external_echo_abs,
			setup_external_echo_abs_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_external_true_abs,
			setup_external_true_abs_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_external_false_abs,
			setup_external_false_abs_ast, teardown_free_cmd_ast),
		cmocka_unit_test_setup_teardown(test_eval_external_ls_abs,
			setup_external_ls_abs_ast, teardown_free_cmd_ast),
	};
	printf("\n--- external commands Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_pipe_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_pipe_two_stage,
			setup_pipe_two_stage, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_file_cat_wc,
			setup_pipe_file_cat_wc, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_long_chain,
			setup_pipe_long_chain, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_large_buffer,
			setup_pipe_large_buffer, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_builtin_to_external,
			setup_pipe_builtin_to_external, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_exit_code_success,
			setup_pipe_exit_code_success, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_exit_code_fail,
			setup_pipe_exit_code_fail, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_error_propagation,
			setup_pipe_error_propagation, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_mid_cmd_not_found,
			setup_pipe_mid_cmd_not_found, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_env_grep,
			setup_pipe_env_grep, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_single_command,
			setup_pipe_single_command, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_very_long_chain,
			setup_pipe_very_long_chain, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_sigpipe,
			setup_pipe_sigpipe_test, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_builtin_pwd_to_cat,
			setup_pipe_builtin_pwd_to_cat, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_cd_in_pipeline,
			setup_pipe_cd_in_pipeline, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_exit_in_middle,
			setup_pipe_exit_in_middle, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_cat_file_to_wc,
			setup_pipe_cat_file_to_wc, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_export_in_pipeline,
			setup_pipe_export_in_pipeline, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_notfound_first,
			setup_pipe_notfound_first, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_notfound_last,
			setup_pipe_notfound_last, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_grep_chain,
			setup_pipe_grep_chain, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_sort_uniq,
			setup_pipe_sort_uniq, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_grep_inverse,
			setup_pipe_grep_inverse, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_mixed_paths,
			setup_pipe_mixed_paths, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_special_chars,
			setup_pipe_special_chars, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_head_tail,
			setup_pipe_head_tail, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_tr_uppercase,
			setup_pipe_tr_uppercase, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_cut_field,
			setup_pipe_cut_field, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_awk_field,
			setup_pipe_awk_field, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_sed_substitute,
			setup_pipe_sed_substitute, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_xargs, setup_pipe_xargs,
			teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_tee, setup_pipe_tee,
			teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_rev, setup_pipe_rev,
			teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_wc_words,
			setup_pipe_wc_words, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_ls_grep,
			setup_pipe_ls_grep, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_multiple_cats,
			setup_pipe_multiple_cats, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_empty_output,
			setup_pipe_empty_output, teardown_free_pipe_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_printf,
			setup_pipe_printf, teardown_free_pipe_ast),
	};
	printf("\n--- pipe Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_redir_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_redir_output_create,
			setup_redir_output_create, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_output_content,
			setup_redir_output_content, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_output_truncate,
			setup_redir_output_truncate, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_append_create,
			setup_redir_append_create, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_append_content,
			setup_redir_append_content, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_input_read,
			setup_redir_input_read, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_input_nonexistent,
			setup_redir_input_nonexistent, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_input_output,
			setup_redir_input_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_multiple_output,
			setup_redir_multiple_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_output_invalid_path,
			setup_redir_output_invalid_path, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_builtin_output,
			setup_redir_builtin_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_env_output,
			setup_redir_env_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_external_input,
			setup_redir_external_input, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_external_both,
			setup_redir_external_both, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_append_no_newline,
			setup_redir_append_no_newline, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_output,
			setup_redir_prefix_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_input,
			setup_redir_prefix_input, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_complex,
			setup_redir_prefix_complex, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_append,
			setup_redir_prefix_append, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_output_append,
			setup_redir_prefix_output_append, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_multiple_output,
			setup_redir_prefix_multiple_output, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_multiple_input,
			setup_redir_prefix_multiple_input, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_builtin,
			setup_redir_prefix_builtin, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_builtin_pwd,
			setup_redir_prefix_builtin_pwd, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_external,
			setup_redir_prefix_external, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_suffix_mixed,
			setup_redir_prefix_suffix_mixed, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_invalid_path,
			setup_redir_prefix_invalid_path, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_input_nonexistent,
			setup_redir_prefix_input_nonexistent, teardown_free_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_redir_prefix_multiple_append,
			setup_redir_prefix_multiple_append, teardown_free_redir_ast),
	};
	printf("\n--- redirection Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_pipe_redir_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_pipe_redir_input_to_wc,
			setup_pipe_redir_input_to_wc, teardown_free_pipe_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_redir_output,
			setup_pipe_redir_output, teardown_free_pipe_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_redir_in_out,
			setup_pipe_redir_in_out, teardown_free_pipe_redir_ast),
		cmocka_unit_test_setup_teardown(test_eval_pipe_redir_append,
			setup_pipe_redir_append, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_input_to_wc,
            setup_pipe_redir_prefix_input_to_wc, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_output,
            setup_pipe_redir_prefix_output, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_in_out,
            setup_pipe_redir_prefix_in_out, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_append,
            setup_pipe_redir_prefix_append, teardown_free_pipe_redir_ast),
//        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_complex,
//            setup_pipe_redir_prefix_complex, teardown_free_pipe_redir_ast),
//        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_override_pipe,
//            setup_pipe_redir_prefix_override_pipe, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_suffix_mixed,
            setup_pipe_redir_prefix_suffix_mixed, teardown_free_pipe_redir_ast),
        cmocka_unit_test_setup_teardown(test_eval_pipe_redir_prefix_input_nonexistent,
            setup_pipe_redir_prefix_input_nonexistent, teardown_free_pipe_redir_ast),
	};
	printf("\n--- pipe with redirection Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static int	run_heredoc_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_cat_basic,
			setup_heredoc_exec_cat_basic, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_with_output,
			setup_heredoc_exec_with_output, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_wc,
			setup_heredoc_exec_wc, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_grep,
			setup_heredoc_exec_grep, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_prefix,
			setup_heredoc_exec_prefix, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_exec_empty,
			setup_heredoc_exec_empty, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_pipe_grep,
			setup_heredoc_pipe_grep, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_multi_pipe,
			setup_heredoc_multi_pipe, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_redir_and_pipe,
			setup_heredoc_redir_and_pipe, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_pipe_sort_uniq,
			setup_heredoc_pipe_sort_uniq, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_pipe_head,
			setup_heredoc_pipe_head, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_multiple,
			setup_heredoc_multiple, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_with_input_redir,
			setup_heredoc_with_input_redir, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_pipe_tr,
			setup_heredoc_pipe_tr, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_special_chars_content,
			setup_heredoc_special_chars_content, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_long_content,
			setup_heredoc_long_content, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_whitespace_lines,
			setup_heredoc_whitespace_lines, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_limiter_in_content,
			setup_heredoc_limiter_in_content, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_numeric_limiter,
			setup_heredoc_numeric_limiter, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_single_char_limiter,
			setup_heredoc_single_char_limiter, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_with_env_vars,
			setup_heredoc_with_env_vars, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_empty_lines,
			setup_heredoc_empty_lines, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_tabs_content,
			setup_heredoc_tabs_content, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_unicode_content,
			setup_heredoc_unicode_content, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_triple_redirect,
			setup_heredoc_triple_redirect, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_after_append,
			setup_heredoc_after_append, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_sed_substitute,
			setup_heredoc_sed_substitute, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_awk_process,
			setup_heredoc_awk_process, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_tee_redirect,
			setup_heredoc_tee_redirect, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_nested_pipes,
			setup_heredoc_nested_pipes, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_parallel_commands,
			setup_heredoc_parallel_commands, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_error_in_pipe,
			setup_heredoc_error_in_pipe, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_partial_match_limiter,
			setup_heredoc_partial_match_limiter, teardown_free_heredoc_ast),
		cmocka_unit_test_setup_teardown(test_eval_heredoc_case_sensitive_limiter,
			setup_heredoc_case_sensitive_limiter, teardown_free_heredoc_ast),
	};
	printf("\n--- heredoc Tests ---\n");
	return (cmocka_run_group_tests(tests, NULL, NULL));
}

static void	print_usage(void)
{
	printf("Usage: ./run_tests [OPTIONS]\n");
	printf("Options:\n");
	printf("  (no args)    Run all tests\n");
	printf("  lexer        Run lexer tests only\n");
	printf("  cd           Run cd builtin tests only\n");
	printf("  echo         Run echo builtin tests only\n");
	printf("  env          Run env  builtin tests only\n");
	printf("  exit         Run exit builtin tests only\n");
	printf("  export       Run export builtin tests only\n");
	printf("  pwd          Run pwd builtin tests only\n");
	printf("  unset        Run unset builtin tests only\n");
	printf("  external     Run external commands tests only\n");
	printf("  pipe         Run pipe tests only\n");
	printf("  redir        Run redirection tests only\n");
	printf("  piredir       Run pipe with redirection tests only\n");
	printf("  heredoc      Run heredoc tests only\n");
	printf("  -h, --help   Show this help message\n");
}
int	main(int argc, char *argv[])
{
	int	result;
	int	failed;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			print_usage();
			return (0);
		}
		if (strcmp(argv[1], "lexer") == 0)
		{
			printf("\n=== Running lexer Tests ===\n");
			return (run_lexer_tests());
		}
		if (strcmp(argv[1], "parser") == 0)
		{
			printf("\n=== Running parser Tests ===\n");
			return (run_parser_tests());
		}
		if (strcmp(argv[1], "signal") == 0)
		{
			printf("\n=== Running signal Tests ===\n");
			return (run_signal_tests());
		}
		if (strcmp(argv[1], "echo") == 0)
		{
			printf("\n=== Running echo Builtin Tests ===\n");
			return (run_echo_builtin_tests());
		}
		if (strcmp(argv[1], "pwd") == 0)
		{
			printf("\n=== Running pwd Builtin Tests ===\n");
			return (run_pwd_builtin_tests());
		}
		if (strcmp(argv[1], "cd") == 0)
		{
			printf("\n=== Running cd Builtin Tests ===\n");
			return (run_cd_builtin_tests());
		}
		if (strcmp(argv[1], "env") == 0)
		{
			printf("\n=== Running env Builtin Tests ===\n");
			return (run_env_builtin_tests());
		}
		if (strcmp(argv[1], "exit") == 0)
		{
			printf("\n=== Running exit Builtin Tests ===\n");
			return (run_exit_builtin_tests());
		}
		if (strcmp(argv[1], "export") == 0)
		{
			printf("\n=== Running export Builtin Tests ===\n");
			return (run_export_builtin_tests());
		}
		if (strcmp(argv[1], "unset") == 0)
		{
			printf("\n=== Running unset Builtin Tests ===\n");
			return (run_unset_builtin_tests());
		}
		if (strcmp(argv[1], "external") == 0)
		{
			printf("\n=== Running External Tests ===\n");
			return (run_external_tests());
		}
		if (strcmp(argv[1], "pipe") == 0)
		{
			printf("\n=== Running Pipe Tests ===\n");
			return (run_pipe_tests());
		}
		if (strcmp(argv[1], "redir") == 0)
		{
			printf("\n=== Running Redirection Tests ===\n");
			return (run_redir_tests());
		}
		if (strcmp(argv[1], "piredir") == 0)
		{
			printf("\n=== Running Pipe with Redirection Tests ===\n");
			return (run_pipe_redir_tests());
		}
		if (strcmp(argv[1], "heredoc") == 0)
		{
			printf("\n=== Running Heredoc Tests ===\n");
			return (run_heredoc_tests());
		}
		printf("Unknown option: %s\n", argv[1]);
		print_usage();
		return (1);
	}
	printf("\n=== Running Minishell Tests ===\n");
	failed = 0;
	result = run_lexer_tests();
	if (result != 0)
		failed += result;
	result = run_parser_tests();
	if (result != 0)
		failed += result;
	result = run_signal_tests();
	if (result != 0)
		failed += result;
	result = run_cd_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_echo_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_env_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_exit_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_export_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_pwd_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_unset_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_external_tests();
	if (result != 0)
		failed += result;
	result = run_pipe_tests();
	if (result != 0)
		failed += result;
	result = run_redir_tests();
	if (result != 0)
		failed += result;
	result = run_pipe_redir_tests();
	if (result != 0)
		failed += result;	
	result = run_heredoc_tests();
	if (result != 0)
		failed += result;	
	printf("\n=== Test Suite Complete ===\n");
	if (failed > 0)
		printf("Total failures: %d\n", failed);
	else
		printf("All tests passed!\n");
	return (failed);
}
