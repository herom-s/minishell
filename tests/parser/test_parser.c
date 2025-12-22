/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:02:36 by thaperei          #+#    #+#             */
/*   Updated: 2025/12/22 11:08:40 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ast.h"
#include "lexer.h"

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

typedef struct s_parse_result
{
	t_ast		*ast;
	t_parser	*parser;
	t_lexer		*lexer;
}	t_parse_result;

t_parse_result *parse_input(const char *input)
{
	t_parse_result *result = malloc(sizeof(t_parse_result));

	result->lexer = create_lexer((char *)input);
	result->parser = create_parser(result->lexer);
	result->ast = init_ast(result->parser);

	return result;
}

void free_parse_result(t_parse_result *result)
{
	if (!result)
		return;

	if (result->ast)
		free_ast(result->ast);
	if (result->parser)
		free(result->parser);
	if (result->lexer)
	{
		ft_lstclear(&(result->lexer->tokens), &free_token);
		free(result->lexer);
	}
	free(result);
}

void print_parser_ast(t_parse_result *result)
{
	ft_printf("\n=== AST Structure ===\n");
	if (result && result->ast)
		print_ast(result->ast, 0);
	else
		ft_printf("NULL or ERROR\n");
	ft_printf("=====================\n");
}

// ============================================================================
// SIMPLE COMMAND TESTS
// ============================================================================

void test_parser_simple_command_single_word(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_name);
	assert_string_equal(result->ast->u_ast.s_simple_cmd.cmd_name, "echo");
	assert_null(result->ast->u_ast.s_simple_cmd.cmd_prefix);
	assert_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	free_parse_result(result);
}

void test_parser_simple_command_with_args(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello world");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_name);
	assert_string_equal(result->ast->u_ast.s_simple_cmd.cmd_name, "echo");
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	t_ast *suffix = result->ast->u_ast.s_simple_cmd.cmd_suffix;
	assert_int_equal(suffix->type, AST_CMD_SUFFIX);
	assert_non_null(suffix->u_ast.s_cmd_suffix.word);
	assert_string_equal(suffix->u_ast.s_cmd_suffix.word, "hello");

	free_parse_result(result);
}

void test_parser_simple_command_with_path(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("/bin/ls -la");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_name);
	assert_string_equal(result->ast->u_ast.s_simple_cmd.cmd_name, "/bin/ls");

	free_parse_result(result);
}

void test_parser_command_multiple_args(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("ls -l -a -h");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_simple_cmd.cmd_name, "ls");
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	free_parse_result(result);
}

// ============================================================================
// PIPELINE TESTS
// ============================================================================

void test_parser_simple_pipe(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello | grep hello");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_PIPE_SEQ);

	assert_non_null(result->ast->u_ast.s_pipe_seq.left);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.left->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_pipe_seq.left->u_ast.s_simple_cmd.cmd_name, "echo");

	assert_non_null(result->ast->u_ast.s_pipe_seq.right);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.right->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_pipe_seq.right->u_ast.s_simple_cmd.cmd_name, "grep");

	free_parse_result(result);
}

void test_parser_multiple_pipes(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("cat file.txt | grep test | wc -l");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_PIPE_SEQ);

	assert_non_null(result->ast->u_ast.s_pipe_seq.left);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.left->type, AST_PIPE_SEQ);

	assert_non_null(result->ast->u_ast.s_pipe_seq.right);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.right->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_pipe_seq.right->u_ast.s_simple_cmd.cmd_name, "wc");

	free_parse_result(result);
}

// ============================================================================
// REDIRECTION TESTS
// ============================================================================

void test_parser_output_redirect(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello > output.txt");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	t_ast *suffix = result->ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix && suffix->type == AST_CMD_SUFFIX)
	{
		if (suffix->u_ast.s_cmd_suffix.io_file)
		{
			t_ast *io = suffix->u_ast.s_cmd_suffix.io_file;
			assert_int_equal(io->type, AST_IO_FILE);
			assert_int_equal(io->u_ast.s_io_file.op->type, GREAT);
			assert_string_equal(io->u_ast.s_io_file.filename, "output.txt");
			break;
		}
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}

	free_parse_result(result);
}

void test_parser_input_redirect(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("cat < input.txt");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);

	t_ast *prefix = result->ast->u_ast.s_simple_cmd.cmd_prefix;
	if (prefix)
	{
		assert_int_equal(prefix->type, AST_CMD_PREFIX);
		assert_non_null(prefix->u_ast.s_cmd_prefix.io_file);

		t_ast *io = prefix->u_ast.s_cmd_prefix.io_file;
		assert_int_equal(io->type, AST_IO_FILE);
		assert_int_equal(io->u_ast.s_io_file.op->type, LESS);
		assert_string_equal(io->u_ast.s_io_file.filename, "input.txt");
	}

	free_parse_result(result);
}

void test_parser_append_redirect(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo test >> output.txt");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	t_ast *suffix = result->ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix && suffix->type == AST_CMD_SUFFIX)
	{
		if (suffix->u_ast.s_cmd_suffix.io_file)
		{
			t_ast *io = suffix->u_ast.s_cmd_suffix.io_file;
			assert_int_equal(io->type, AST_IO_FILE);
			assert_int_equal(io->u_ast.s_io_file.op->type, DGREAT);
			assert_string_equal(io->u_ast.s_io_file.filename, "output.txt");
			break;
		}
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}

	free_parse_result(result);
}

void test_parser_heredoc(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("cat << EOF");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);

	t_ast *prefix = result->ast->u_ast.s_simple_cmd.cmd_prefix;
	if (prefix && prefix->u_ast.s_cmd_prefix.io_file)
	{
		t_ast *io = prefix->u_ast.s_cmd_prefix.io_file;
		assert_int_equal(io->type, AST_IO_FILE);
		assert_int_equal(io->u_ast.s_io_file.op->type, DLESS);
		assert_string_equal(io->u_ast.s_io_file.filename, "EOF");
	}

	free_parse_result(result);
}

void test_parser_multiple_redirects(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("cat < input.txt > output.txt");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_prefix);
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	free_parse_result(result);
}

// ============================================================================
// LOGICAL OPERATORS TESTS (&&, ||)
// ============================================================================

void test_parser_and_operator(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello && echo world");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.op);
	assert_int_equal(result->ast->u_ast.s_and_or.op->type, AND_IF);

	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_int_equal(result->ast->u_ast.s_and_or.left->type, AST_SIMPLE_CMD);

	assert_non_null(result->ast->u_ast.s_and_or.right);
	assert_int_equal(result->ast->u_ast.s_and_or.right->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

void test_parser_or_operator(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello || echo world");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.op);
	assert_int_equal(result->ast->u_ast.s_and_or.op->type, OR_IF);

	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_non_null(result->ast->u_ast.s_and_or.right);

	free_parse_result(result);
}

void test_parser_mixed_logical_operators(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo a && echo b || echo c");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_int_equal(result->ast->u_ast.s_and_or.left->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.right);

	free_parse_result(result);
}

void test_parser_precedence_and_vs_pipe(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo a | grep a && echo b");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);

	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_int_equal(result->ast->u_ast.s_and_or.left->type, AST_PIPE_SEQ);

	assert_non_null(result->ast->u_ast.s_and_or.right);
	assert_int_equal(result->ast->u_ast.s_and_or.right->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

// ============================================================================
// SUBSHELL TESTS (BONUS)
// ============================================================================

void test_parser_simple_subshell(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo hello)");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_subshell.and_or);
	assert_int_equal(result->ast->u_ast.s_subshell.and_or->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_subshell.and_or->u_ast.s_simple_cmd.cmd_name, "echo");

	free_parse_result(result);
}

void test_parser_subshell_with_pipe(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo hello | grep h)");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_subshell.and_or);
	assert_int_equal(result->ast->u_ast.s_subshell.and_or->type, AST_PIPE_SEQ);

	free_parse_result(result);
}

void test_parser_nested_subshells(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("((echo hello))");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_subshell.and_or);
	assert_int_equal(result->ast->u_ast.s_subshell.and_or->type, AST_SUBSHELL);

	t_ast *inner = result->ast->u_ast.s_subshell.and_or;
	assert_non_null(inner->u_ast.s_subshell.and_or);
	assert_int_equal(inner->u_ast.s_subshell.and_or->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

void test_parser_subshell_with_logical_ops(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo a && echo b)");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_subshell.and_or);
	assert_int_equal(result->ast->u_ast.s_subshell.and_or->type, AST_AND_OR);

	free_parse_result(result);
}

// ============================================================================
// COMPLEX COMBINATIONS
// ============================================================================

void test_parser_pipe_with_redirects(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("cat < input.txt | grep test > output.txt");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_PIPE_SEQ);
	assert_non_null(result->ast->u_ast.s_pipe_seq.left);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.left->type, AST_SIMPLE_CMD);
	assert_non_null(result->ast->u_ast.s_pipe_seq.right);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.right->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

void test_parser_logical_with_pipes(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo a | grep a && echo b | grep b");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_int_equal(result->ast->u_ast.s_and_or.left->type, AST_PIPE_SEQ);
	assert_non_null(result->ast->u_ast.s_and_or.right);
	assert_int_equal(result->ast->u_ast.s_and_or.right->type, AST_PIPE_SEQ);

	free_parse_result(result);
}

void test_parser_subshell_in_pipeline(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo hello) | grep h");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_PIPE_SEQ);
	assert_non_null(result->ast->u_ast.s_pipe_seq.left);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.left->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_pipe_seq.right);
	assert_int_equal(result->ast->u_ast.s_pipe_seq.right->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

void test_parser_subshell_and_logical(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo a) && (echo b)");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_AND_OR);
	assert_non_null(result->ast->u_ast.s_and_or.left);
	assert_int_equal(result->ast->u_ast.s_and_or.left->type, AST_SUBSHELL);
	assert_non_null(result->ast->u_ast.s_and_or.right);
	assert_int_equal(result->ast->u_ast.s_and_or.right->type, AST_SUBSHELL);

	free_parse_result(result);
}

// ============================================================================
// EDGE CASES AND ERROR HANDLING
// ============================================================================

void test_parser_empty_input(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("");
	assert_non_null(result);

	free_parse_result(result);
}

void test_parser_only_pipe(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("|");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_pipe_without_right_side(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello |");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_redirect_without_filename(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello >");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_unclosed_subshell(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("(echo hello");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_unopened_subshell(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello)");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_double_pipe(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo | | cat");
	assert_non_null(result);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

void test_parser_double_redirect(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo > > file");
	assert_null(result->ast);
	assert_int_equal(result->parser->has_error, 1);

	free_parse_result(result);
}

// ============================================================================
// QUOTE HANDLING
// ============================================================================

void test_parser_command_with_quoted_args(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo 'hello world'");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);
	assert_string_equal(result->ast->u_ast.s_simple_cmd.cmd_name, "echo");
	assert_non_null(result->ast->u_ast.s_simple_cmd.cmd_suffix);

	free_parse_result(result);
}

void test_parser_redirect_with_quoted_filename(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo hello > \"output file.txt\"");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}

void test_parser_mixed_quotes(void **state)
{
	(void)state;

	t_parse_result *result = parse_input("echo \"hello 'world'\"");
	assert_non_null(result);
	assert_non_null(result->ast);
	assert_int_equal(result->parser->has_error, 0);

	assert_int_equal(result->ast->type, AST_SIMPLE_CMD);

	free_parse_result(result);
}
