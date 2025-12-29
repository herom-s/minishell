/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:14:46 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/16 14:15:21 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_eval.h"
#include "tests.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

static int	run_lexer_tests(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_one_char_operators),
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
	};
	printf("\n--- external commands Tests ---\n");
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
		printf("Unknown option: %s\n", argv[1]);
		print_usage();
		return (1);
	}
	printf("\n=== Running Minishell Tests ===\n");
	failed = 0;
	result = run_lexer_tests();
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
	printf("\n=== Test Suite Complete ===\n");
	if (failed > 0)
		printf("Total failures: %d\n", failed);
	else
		printf("All tests passed!\n");
	return (failed);
}
