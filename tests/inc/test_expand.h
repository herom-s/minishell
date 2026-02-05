/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expand.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_EXPAND_H
# define TEST_EXPAND_H

# include "expand.h"
# include "eval.h"
# include <setjmp.h>
# include <stdarg.h>
# include <stddef.h>
# include <cmocka.h>

/* Setup and teardown */
int		setup_expand_test(void **state);
int		teardown_expand_test(void **state);

/* Variable expansion tests */
void	test_expand_simple_variable(void **state);
void	test_expand_undefined_variable(void **state);
void	test_expand_exit_status(void **state);
void	test_expand_shell_pid(void **state);
void	test_expand_shell_name(void **state);
void	test_expand_dollar_at_end(void **state);
void	test_expand_dollar_with_invalid_char(void **state);
void	test_expand_adjacent_variables(void **state);
void	test_expand_variable_in_double_quotes(void **state);
void	test_expand_variable_in_single_quotes(void **state);

/* Quote handling tests */
void	test_remove_single_quotes(void **state);
void	test_remove_double_quotes(void **state);
void	test_expand_mixed_quotes(void **state);
void	test_expand_nested_quotes(void **state);
void	test_quote_state_tracking(void **state);

/* Wildcard expansion tests */
void	test_wildcard_detection(void **state);
void	test_pattern_matching_star_only(void **state);
void	test_pattern_matching_prefix(void **state);
void	test_pattern_matching_suffix(void **state);
void	test_pattern_matching_middle(void **state);
void	test_wildcard_in_quotes_not_expanded(void **state);
void	test_wildcard_sorting(void **state);

#endif
