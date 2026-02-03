/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:44:09 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/29 18:00:54 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

static int	check_overflow(unsigned long long res, int digit, int sign,
				int next)
{
	if (res > (unsigned long long)LLONG_MAX / 10)
		return (0);
	if (res == (unsigned long long)LLONG_MAX / 10 && digit > (LLONG_MAX % 10))
	{
		if (sign == -1 && digit == 8 && !next)
			return (1);
		return (0);
	}
	return (1);
}

static int	check_exit_arg(char *arg)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	if (!arg[i])
		return (0);
	result = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		if (!check_overflow(result, arg[i] - '0', sign, arg[i + 1]))
			return (0);
		result = (result * 10) + (arg[i] - '0');
		i++;
	}
	return (1);
}

static void	exit_no_args(t_ast *ast, t_shell_env *env, t_cmd_response *res)
{
	int	exit_code;

	exit_code = 0;
	if (env)
		exit_code = env->last_exit_code;
	destroy_cmd_res(res);
	child_exit(env, ast, exit_code);
}

static void	exit_invalid_arg(t_ast *ast, char **cmd, t_shell_env *env,
					t_cmd_response *res)
{
	ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n",
		cmd[1]);
	destroy_cmd_res(res);
	child_exit(env, ast, 2);
}

t_cmd_response	*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	size_t			num_args;

	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (env && env->interactive_owner && isatty(STDIN_FILENO))
		ft_dprintf(STDERR_FILENO, "exit\n");
	num_args = num_arguments(cmd_str);
	if (num_args == 0)
		exit_no_args(shell_ast, env, res);
	if (!check_exit_arg(cmd_str[1]))
		exit_invalid_arg(shell_ast, cmd_str, env, res);
	if (num_args >= 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		res->exit_code = 1;
		return (res);
	}
	destroy_cmd_res(res);
	child_exit(env, shell_ast, (unsigned char)ft_atoll(cmd_str[1]));
	return (NULL);
}
