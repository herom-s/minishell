/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expand_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_expand.h"
#include "expand.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

void	test_remove_single_quotes(void **state)
{
	char	*result;

	(void)state;
	result = remove_quotes("'hello world'");
	assert_string_equal(result, "hello world");
	free(result);
}

void	test_remove_double_quotes(void **state)
{
	char	*result;

	(void)state;
	result = remove_quotes("\"hello world\"");
	assert_string_equal(result, "hello world");
	free(result);
}

void	test_expand_mixed_quotes(void **state)
{
	char	*result;

	(void)state;
	result = remove_quotes("'hello'\"world\"");
	assert_string_equal(result, "helloworld");
	free(result);
}

void	test_expand_nested_quotes(void **state)
{
	char	*result;

	(void)state;
	result = remove_quotes("\"'hello'\"");
	assert_string_equal(result, "'hello'");
	free(result);
	result = remove_quotes("'\"hello\"'");
	assert_string_equal(result, "\"hello\"");
	free(result);
}

void	test_quote_state_tracking(void **state)
{
	t_quote_state	q_state;

	(void)state;
	q_state = QUOTE_NONE;
	q_state = update_quote_state('\'', q_state);
	assert_int_equal(q_state, QUOTE_SINGLE);
	q_state = update_quote_state('\'', q_state);
	assert_int_equal(q_state, QUOTE_NONE);
	q_state = update_quote_state('"', q_state);
	assert_int_equal(q_state, QUOTE_DOUBLE);
	q_state = update_quote_state('"', q_state);
	assert_int_equal(q_state, QUOTE_NONE);
	q_state = update_quote_state('\'', q_state);
	q_state = update_quote_state('"', q_state);
	assert_int_equal(q_state, QUOTE_SINGLE);
}
