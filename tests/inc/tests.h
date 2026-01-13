/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 11:57:12 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/05 18:01:57 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

//	tests for lexer
void	test_one_char_operators(void **state);

// setup for echo eval
int		setup_built_in_echo_n_basic_ast(void **state);
int		setup_built_in_echo_basic_ast(void **state);
int		setup_built_in_echo_empty_ast(void **state);
int		setup_built_in_echo_n_empty_ast(void **state);
int		setup_built_in_echo_mutiple_args_ast(void **state);
int		setup_built_in_echo_n_mutiple_args_ast(void **state);

// tests for echo eval
void	test_eval_built_in_echo_n_basic(void **state);
void	test_eval_built_in_echo_basic(void **state);
void	test_eval_built_in_echo_empty(void **state);
void	test_eval_built_in_echo_n_empty(void **state);
void	test_eval_built_in_echo_mutiple_args(void **state);
void	test_eval_built_in_echo_n_mutiple_args(void **state);

// setup for pwd eval
int		setup_built_in_pwd_basic_ast(void **state);

// tests for pwd eval
void	test_eval_built_in_pwd_basic(void **stati);

// setup for pwd eval
int		setup_built_in_cd_path_ast(void **state);
int		setup_built_in_cd_to_many_args_ast(void **state);

// tests for pwd eval
void	test_eval_built_in_cd_path(void **state);
void	test_eval_built_in_cd_to_many_args(void **state);

// setup for env eval
int		setup_built_in_env_no_args_ast(void **state);

// tests for env eval
void	test_eval_built_in_env_no_args(void **state);
void	test_eval_built_in_env_not_empty(void **state);
void	test_eval_built_in_env_contains_path(void **state);

// setup for exit eval
int		setup_built_in_exit_single_arg_ast(void **state);
int		setup_built_in_exit_to_many_args_ast(void **state);

// tests for exit eval
void	test_eval_built_in_exit_single_arg_path(void **state);
void	test_eval_built_in_exit_to_many_args_path(void **state);

// setup for export eval
int		setup_built_in_export_no_args_ast(void **state);
int		setup_built_in_export_arg_ast(void **state);
int		setup_built_in_export_mutiple_args_ast(void **state);
int		setup_built_in_export_invalid_digit_ast(void **state);
int		setup_built_in_export_invalid_special_ast(void **state);
int		setup_built_in_export_mixed_args_ast(void **state);

// tests for export eval
void	test_eval_built_in_export_no_args(void **state);
void	test_eval_built_in_export_arg(void **state);
void	test_eval_built_in_export_mutiple_args(void **state);
void	test_eval_built_in_export_invalid_digit(void **state);
void	test_eval_built_in_export_invalid_special(void **state);
void	test_eval_built_in_export_mixed_args(void **state);

// setup for unset eval
int		setup_built_in_unset_no_args_ast(void **state);
int		setup_built_in_unset_single_var_ast(void **state);
int		setup_built_in_unset_multiple_vars_ast(void **state);
int		setup_built_in_unset_nonexistent_ast(void **state);

// tests for unset eval
void	test_eval_built_in_unset_no_args(void **state);
void	test_eval_built_in_unset_single_var(void **state);
void	test_eval_built_in_unset_multiple_vars(void **state);
void	test_eval_built_in_unset_nonexistent(void **state);

// setup for external cmd eval
int		setup_external_cat_abs_ast(void **state);
int		setup_external_invalid_abs_ast(void **state);
int		setup_external_echo_abs_ast(void **state);
int		setup_external_true_abs_ast(void **state);
int		setup_external_false_abs_ast(void **state);
int		setup_external_ls_abs_ast(void **state);

// tests for external cmd eval
void	test_eval_external_cat_abs(void **state);
void	test_eval_external_invalid_abs(void **state);
void	test_eval_external_echo_abs(void **state);
void	test_eval_external_true_abs(void **state);
void	test_eval_external_false_abs(void **state);
void	test_eval_external_ls_abs(void **state);

// setup for exernal pipe cmd eval
int		setup_pipe_two_stage(void **state);
int		setup_pipe_file_cat_wc(void **state);
int		setup_pipe_long_chain(void **state);
int		setup_pipe_large_buffer(void **state);
int		setup_pipe_builtin_to_external(void **state);
int		setup_pipe_exit_code_success(void **state);
int		setup_pipe_exit_code_fail(void **state);
int		setup_pipe_error_propagation(void **state);
int		setup_pipe_mid_cmd_not_found(void **state);
int		setup_pipe_env_grep(void **state);
int		setup_pipe_single_command(void **state);
int		setup_pipe_very_long_chain(void **state);
int		setup_pipe_sigpipe_test(void **state);
int		setup_pipe_builtin_pwd_to_cat(void **state);
int		setup_pipe_cd_in_pipeline(void **state);
int		setup_pipe_exit_in_middle(void **state);
int		setup_pipe_cat_file_to_wc(void **state);
int		setup_pipe_export_in_pipeline(void **state);
int		setup_pipe_notfound_first(void **state);
int		setup_pipe_notfound_last(void **state);
int		setup_pipe_grep_chain(void **state);
int		setup_pipe_sort_uniq(void **state);
int		setup_pipe_grep_inverse(void **state);
int		setup_pipe_mixed_paths(void **state);
int		setup_pipe_special_chars(void **state);
int		setup_pipe_head_tail(void **state);
int		setup_pipe_tr_uppercase(void **state);
int		setup_pipe_cut_field(void **state);
int		setup_pipe_awk_field(void **state);
int		setup_pipe_sed_substitute(void **state);
int		setup_pipe_xargs(void **state);
int		setup_pipe_tee(void **state);
int		setup_pipe_rev(void **state);
int		setup_pipe_wc_words(void **state);
int		setup_pipe_ls_grep(void **state);
int		setup_pipe_multiple_cats(void **state);
int		setup_pipe_empty_output(void **state);
int		setup_pipe_printf(void **state);

// tests for external pipe cmd eval
void	test_eval_pipe_two_stage(void **state);
void	test_eval_pipe_file_cat_wc(void **state);
void	test_eval_pipe_long_chain(void **state);
void	test_eval_pipe_large_buffer(void **state);
void	test_eval_pipe_builtin_to_external(void **state);
void	test_eval_pipe_exit_code_success(void **state);
void	test_eval_pipe_exit_code_fail(void **state);
void	test_eval_pipe_error_propagation(void **state);
void	test_eval_pipe_mid_cmd_not_found(void **state);
void	test_eval_pipe_env_grep(void **state);
void	test_eval_pipe_single_command(void **state);
void	test_eval_pipe_very_long_chain(void **state);
void	test_eval_pipe_sigpipe(void **state);
void	test_eval_pipe_builtin_pwd_to_cat(void **state);
void	test_eval_pipe_cd_in_pipeline(void **state);
void	test_eval_pipe_exit_in_middle(void **state);
void	test_eval_pipe_cat_file_to_wc(void **state);
void	test_eval_pipe_export_in_pipeline(void **state);
void	test_eval_pipe_notfound_first(void **state);
void	test_eval_pipe_notfound_last(void **state);
void	test_eval_pipe_grep_chain(void **state);
void	test_eval_pipe_sort_uniq(void **state);
void	test_eval_pipe_grep_inverse(void **state);
void	test_eval_pipe_mixed_paths(void **state);
void	test_eval_pipe_special_chars(void **state);
void	test_eval_pipe_head_tail(void **state);
void	test_eval_pipe_tr_uppercase(void **state);
void	test_eval_pipe_cut_field(void **state);
void	test_eval_pipe_awk_field(void **state);
void	test_eval_pipe_sed_substitute(void **state);
void	test_eval_pipe_xargs(void **state);
void	test_eval_pipe_tee(void **state);
void	test_eval_pipe_rev(void **state);
void	test_eval_pipe_wc_words(void **state);
void	test_eval_pipe_ls_grep(void **state);
void	test_eval_pipe_multiple_cats(void **state);
void	test_eval_pipe_empty_output(void **state);
void	test_eval_pipe_printf(void **state);

// setup for redir eval
int		setup_redir_output_create(void **state);
int		setup_redir_output_content(void **state);
int		setup_redir_output_truncate(void **state);
int		setup_redir_append_create(void **state);
int		setup_redir_append_content(void **state);
int		setup_redir_input_read(void **state);
int		setup_redir_input_nonexistent(void **state);
int		setup_redir_input_output(void **state);
int		setup_redir_multiple_output(void **state);
int		setup_redir_output_invalid_path(void **state);
int		setup_redir_builtin_output(void **state);
int		setup_redir_env_output(void **state);
int		setup_redir_external_input(void **state);
int		setup_redir_external_both(void **state);
int		setup_redir_append_no_newline(void **state);
int		setup_redir_prefix_output(void **state);
int		setup_redir_prefix_input(void **state);
int		setup_redir_prefix_complex(void **state);
int     setup_redir_prefix_output_append(void **state);
int     setup_redir_prefix_append(void **state);
int     setup_redir_prefix_multiple_output(void **state);
int     setup_redir_prefix_multiple_input(void **state);
int     setup_redir_prefix_builtin(void **state);
int     setup_redir_prefix_builtin_pwd(void **state);
int     setup_redir_prefix_external(void **state);
int     setup_redir_prefix_suffix_mixed(void **state);
int     setup_redir_prefix_invalid_path(void **state);
int     setup_redir_prefix_input_nonexistent(void **state);
int     setup_redir_prefix_multiple_append(void **state);

// tests for redir eval
void	test_eval_redir_output_create(void **state);
void	test_eval_redir_output_content(void **state);
void	test_eval_redir_output_truncate(void **state);
void	test_eval_redir_append_create(void **state);
void	test_eval_redir_append_content(void **state);
void	test_eval_redir_input_read(void **state);
void	test_eval_redir_input_nonexistent(void **state);
void	test_eval_redir_input_output(void **state);
void	test_eval_redir_multiple_output(void **state);
void	test_eval_redir_output_invalid_path(void **state);
void	test_eval_redir_builtin_output(void **state);
void	test_eval_redir_env_output(void **state);
void	test_eval_redir_external_input(void **state);
void	test_eval_redir_external_both(void **state);
void	test_eval_redir_append_no_newline(void **state);
void	test_eval_redir_prefix_output(void **state);
void	test_eval_redir_prefix_input(void **state);
void	test_eval_redir_prefix_complex(void **state);
void    test_eval_redir_prefix_output_append(void **state);
void    test_eval_redir_prefix_append(void **state);
void    test_eval_redir_prefix_multiple_output(void **state);
void    test_eval_redir_prefix_multiple_input(void **state);
void    test_eval_redir_prefix_builtin(void **state);
void    test_eval_redir_prefix_builtin_pwd(void **state);
void    test_eval_redir_prefix_external(void **state);
void    test_eval_redir_prefix_suffix_mixed(void **state);
void    test_eval_redir_prefix_invalid_path(void **state);
void    test_eval_redir_prefix_input_nonexistent(void **state);
void    test_eval_redir_prefix_multiple_append(void **state);

// setup for redir pipe eval
int		setup_pipe_redir_input_to_wc(void **state);
int		setup_pipe_redir_output(void **state);
int		setup_pipe_redir_in_out(void **state);
int		setup_pipe_redir_append(void **state);
int     setup_pipe_redir_prefix_input_to_wc(void **state);
int     setup_pipe_redir_prefix_output(void **state);
int     setup_pipe_redir_prefix_in_out(void **state);
int     setup_pipe_redir_prefix_append(void **state);
int     setup_pipe_redir_prefix_complex(void **state);
int     setup_pipe_redir_prefix_override_pipe(void **state);
int     setup_pipe_redir_prefix_suffix_mixed(void **state);
int     setup_pipe_redir_prefix_input_nonexistent(void **state);

// setup for redir pipe eval
void	test_eval_pipe_redir_input_to_wc(void **state);
void	test_eval_pipe_redir_output(void **state);
void	test_eval_pipe_redir_in_out(void **state);
void	test_eval_pipe_redir_append(void **state);
void    test_eval_pipe_redir_prefix_input_to_wc(void **state);
void    test_eval_pipe_redir_prefix_output(void **state);
void    test_eval_pipe_redir_prefix_in_out(void **state);
void    test_eval_pipe_redir_prefix_append(void **state);
void    test_eval_pipe_redir_prefix_complex(void **state);
void    test_eval_pipe_redir_prefix_override_pipe(void **state);
void    test_eval_pipe_redir_prefix_suffix_mixed(void **state);
void    test_eval_pipe_redir_prefix_input_nonexistent(void **state);

// setup for heredoc eval
int		setup_heredoc_exec_cat_basic(void **state);
int		setup_heredoc_exec_with_output(void **state);
int		setup_heredoc_exec_wc(void **state);
int		setup_heredoc_exec_grep(void **state);
int		setup_heredoc_exec_prefix(void **state);
int		setup_heredoc_exec_empty(void **state);
int		setup_heredoc_pipe_grep(void **state);
int		setup_heredoc_multi_pipe(void **state);
int		setup_heredoc_redir_and_pipe(void **state);
int		setup_heredoc_pipe_sort_uniq(void **state);
int		setup_heredoc_pipe_head(void **state);
int		setup_heredoc_multiple(void **state);
int		setup_heredoc_with_input_redir(void **state);
int		setup_heredoc_pipe_tr(void **state);
int     setup_heredoc_special_chars_content(void **state);
int     setup_heredoc_long_content(void **state);
int     setup_heredoc_whitespace_lines(void **state);
int     setup_heredoc_limiter_in_content(void **state);
int     setup_heredoc_numeric_limiter(void **state);
int     setup_heredoc_single_char_limiter(void **state);
int     setup_heredoc_with_env_vars(void **state);
int     setup_heredoc_empty_lines(void **state);
int     setup_heredoc_tabs_content(void **state);
int     setup_heredoc_unicode_content(void **state);
int     setup_heredoc_triple_redirect(void **state);
int     setup_heredoc_after_append(void **state);
int     setup_heredoc_sed_substitute(void **state);
int     setup_heredoc_awk_process(void **state);
int     setup_heredoc_tee_redirect(void **state);
int     setup_heredoc_nested_pipes(void **state);
int     setup_heredoc_parallel_commands(void **state);
int     setup_heredoc_error_in_pipe(void **state);
int     setup_heredoc_partial_match_limiter(void **state);
int     setup_heredoc_case_sensitive_limiter(void **state);

// tests for heredoc eval
void	test_eval_heredoc_basic_structure(void **state);
void	test_eval_heredoc_with_args(void **state);
void	test_eval_heredoc_external_cmd(void **state);
void	test_eval_heredoc_prefix(void **state);
void	test_eval_heredoc_multiple(void **state);
void	test_eval_heredoc_with_output_redir(void **state);
void	test_eval_heredoc_quoted_limiter(void **state);
void	test_eval_heredoc_in_pipeline(void **state);
void	test_eval_heredoc_special_limiter(void **state);
void	test_eval_heredoc_exec_cat_basic(void **state);
void	test_eval_heredoc_exec_with_output(void **state);
void	test_eval_heredoc_exec_wc(void **state);
void	test_eval_heredoc_exec_grep(void **state);
void	test_eval_heredoc_exec_prefix(void **state);
void	test_eval_heredoc_exec_empty(void **state);
void	test_eval_heredoc_pipe_grep(void **state);
void	test_eval_heredoc_multi_pipe(void **state);
void	test_eval_heredoc_redir_and_pipe(void **state);
void	test_eval_heredoc_pipe_sort_uniq(void **state);
void	test_eval_heredoc_pipe_head(void **state);
void	test_eval_heredoc_multiple(void **state);
void	test_eval_heredoc_with_input_redir(void **state);
void	test_eval_heredoc_pipe_tr(void **state);
void    test_eval_heredoc_special_chars_content(void **state);
void    test_eval_heredoc_long_content(void **state);
void    test_eval_heredoc_whitespace_lines(void **state);
void    test_eval_heredoc_limiter_in_content(void **state);
void    test_eval_heredoc_numeric_limiter(void **state);
void    test_eval_heredoc_single_char_limiter(void **state);
void    test_eval_heredoc_with_env_vars(void **state);
void    test_eval_heredoc_empty_lines(void **state);
void    test_eval_heredoc_tabs_content(void **state);
void    test_eval_heredoc_unicode_content(void **state);
void    test_eval_heredoc_triple_redirect(void **state);
void    test_eval_heredoc_after_append(void **state);
void    test_eval_heredoc_sed_substitute(void **state);
void    test_eval_heredoc_awk_process(void **state);
void    test_eval_heredoc_tee_redirect(void **state);
void    test_eval_heredoc_nested_pipes(void **state);
void    test_eval_heredoc_parallel_commands(void **state);
void    test_eval_heredoc_error_in_pipe(void **state);
void    test_eval_heredoc_partial_match_limiter(void **state);
void    test_eval_heredoc_case_sensitive_limiter(void **state);

int		teardown_free_heredoc_ast(void **state);
#endif
