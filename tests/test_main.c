/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/12/01 11:57:53 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/01 11:57:55 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "test_eval.h"
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

static void	print_usage(void)
{
	printf("Usage: ./run_tests [OPTIONS]\n");
	printf("Options:\n");
	printf("  (no args)    Run all tests\n");
	printf("  lexer        Run lexer tests only\n");
	printf("  echo         Run echo builtin tests only\n");
	printf("  pwd          Run pwd builtin tests only\n");
	printf("  cd           Run cd builtin tests only\n");
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
		printf("Unknown option: %s\n", argv[1]);
		print_usage();
		return (1);
	}
	printf("\n=== Running Minishell Tests ===\n");
	failed = 0;
	result = run_lexer_tests();
	if (result != 0)
		failed += result;
	result = run_echo_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_pwd_builtin_tests();
	if (result != 0)
		failed += result;
	result = run_cd_builtin_tests();
	if (result != 0)
		failed += result;
	printf("\n=== Test Suite Complete ===\n");
	if (failed > 0)
		printf("Total failures: %d\n", failed);
	else
		printf("All tests passed!\n");
	return (failed);
}
