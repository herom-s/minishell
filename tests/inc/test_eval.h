/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eval.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 12:07:16 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 12:12:42 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_EVAL_H
# define TEST_EVAL_H

#include "ast.h"

typedef struct s_test_cmd_params
{
	char		*cmd_name;
	char		**args;
	int			arg_count;
	char		*expected_output;
	int			expected_exit_code;
}				t_test_cmd_params;


t_ast	*create_cmd_ast(char *cmd_name, char **args, int arg_count);
void	free_cmd_ast(t_ast *ast);

int	teardown_free_cmd_ast(void **state);

#endif
