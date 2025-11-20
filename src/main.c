/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/20 14:55:44 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "print.h"
#include <readline/history.h>
#include <readline/readline.h>

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_memset(&shell, 0, sizeof(t_minishell));
	while (1)
	{
		shell.input = readline("minishell> ");
		add_history(shell.input);
		shell.ast_shell = read_input(shell.input);
		shell.shell_response = eval_ast(shell.ast_shell);
		print_shell_response(shell.shell_response);
		free(shell.input);
	}
	return (0);
}
