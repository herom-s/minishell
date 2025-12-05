/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 11:57:12 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 16:30:09 by hermarti         ###   ########.fr       */
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
int	setup_built_in_cd_path_ast(void **state);
int	setup_built_in_cd_to_many_args_ast(void **state);

// tests for pwd eval
void	test_eval_built_in_cd_path(void **state);
void	test_eval_built_in_cd_to_many_args(void **state);

// setup for exit eval
int	setup_built_in_exit_single_arg_ast(void **state);
int	setup_built_in_exit_to_many_args_ast(void **state);

// tests for exit eval
void	test_eval_built_in_exit_single_arg_path(void **state);
void	test_eval_built_in_exit_to_many_args_path(void **state);

#endif
