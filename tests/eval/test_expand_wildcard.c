/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expand_wildcard.c                             :+:      :+:    :+:   */
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

void	test_wildcard_detection(void **state)
{
	(void)state;
	assert_int_equal(has_wildcard("*.c"), 1);
	assert_int_equal(has_wildcard("file*"), 1);
	assert_int_equal(has_wildcard("*"), 1);
	assert_int_equal(has_wildcard("no_wildcard"), 0);
	assert_int_equal(has_wildcard("'*'"), 0);
	assert_int_equal(has_wildcard("\"*\""), 0);
}

void	test_pattern_matching_star_only(void **state)
{
	(void)state;
	assert_int_equal(match_pattern("*", "anything"), 1);
	assert_int_equal(match_pattern("*", ""), 1);
	assert_int_equal(match_pattern("*", "file.c"), 1);
}

void	test_pattern_matching_prefix(void **state)
{
	(void)state;
	assert_int_equal(match_pattern("test*", "testfile"), 1);
	assert_int_equal(match_pattern("test*", "test"), 1);
	assert_int_equal(match_pattern("test*", "testing"), 1);
	assert_int_equal(match_pattern("test*", "file"), 0);
}

void	test_pattern_matching_suffix(void **state)
{
	(void)state;
	assert_int_equal(match_pattern("*.c", "file.c"), 1);
	assert_int_equal(match_pattern("*.c", "test.c"), 1);
	assert_int_equal(match_pattern("*.c", ".c"), 1);
	assert_int_equal(match_pattern("*.c", "file.h"), 0);
	assert_int_equal(match_pattern("*.c", "filec"), 0);
}

void	test_pattern_matching_middle(void **state)
{
	(void)state;
	assert_int_equal(match_pattern("test*.c", "test_file.c"), 1);
	assert_int_equal(match_pattern("test*.c", "test.c"), 1);
	assert_int_equal(match_pattern("test*.c", "testing.c"), 1);
	assert_int_equal(match_pattern("test*.c", "file.c"), 0);
	assert_int_equal(match_pattern("test*.c", "test.h"), 0);
}

void	test_wildcard_in_quotes_not_expanded(void **state)
{
	(void)state;
	assert_int_equal(has_wildcard("'*.c'"), 0);
	assert_int_equal(has_wildcard("\"*.c\""), 0);
	assert_int_equal(has_wildcard("test'*'.c"), 0);
}

void	test_wildcard_sorting(void **state)
{
	char	*arr[4];

	(void)state;
	arr[0] = ft_strdup("cherry");
	arr[1] = ft_strdup("apple");
	arr[2] = ft_strdup("banana");
	arr[3] = NULL;
	sort_matches(arr, 3);
	assert_string_equal(arr[0], "apple");
	assert_string_equal(arr[1], "banana");
	assert_string_equal(arr[2], "cherry");
	free(arr[0]);
	free(arr[1]);
	free(arr[2]);
}
