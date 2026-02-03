/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:47:49 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include "minishell.h"
#include "minishell_signal.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	parse_input(char *input, t_shell_env *env);

static int	get_input(t_minishell *shell)
{
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_signal;
	shell->input = readline("minishell$ ");
	if (isatty(STDIN_FILENO))
		rl_event_hook = NULL;
	if (g_sig == SIGINT)
	{
		free(shell->input);
		return (1);
	}
	if (!shell->input)
	{
		ft_putstr_fd("exit\n", 1);
		return (-1);
	}
	if (shell->input[0] != '\0')
		add_history(shell->input);
	return (0);
}

static void	process_shell_input(t_minishell *shell, t_shell_env *env)
{
	env->current_input = shell->input;
	parse_input(shell->input, env);
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
		env->current_input = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;
	t_shell_env	*env;
	int			status;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_minishell));
	setup_nonfork_signal();
	env = create_shell_env(envp);
	while (1)
	{
		g_sig = 0;
		status = get_input(&shell);
		if (status == 1)
			continue ;
		if (status == -1)
			break ;
		process_shell_input(&shell, env);
	}
	status = env->last_exit_code;
	destroy_shell_env(env);
	return (status);
}
