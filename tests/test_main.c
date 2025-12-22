/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:14:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/22 10:18:51 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_eval.h"
#include "test_lexer.h"
#include "test_parser.h"
#include "tests.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

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
	printf("\n=== Test Suite Complete ===\n");
	if (failed > 0)
		printf("Total failures: %d\n", failed);
	else
		printf("All tests passed!\n");
	return (failed);
}
