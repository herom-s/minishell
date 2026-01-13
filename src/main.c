/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/07 21:33:22 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "print.h"
#include "minishell_signal.h"
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	parse_input(char *input, char *envp[])
{
	t_shell_env			*env;
	t_shell_response	*res;
	t_lexer				*lexer;
	t_parser			*parser;
	t_ast				*ast;

	(void)envp;
	lexer = create_lexer(input);
	if (lexer == NULL)
	{
		ft_printf("Failed to create lexer\n");
		return ;
	}
	parser = create_parser(lexer);
	if (parser == NULL)
	{
		ft_printf("Failed to create parser\n");
		return (free(lexer));
	}
	env = create_shell_env(envp);
	ast = init_ast(parser);
	if (ast)
	{
		res = eval_ast(ast, env);
		free_ast(ast);
		print_shell_response(res);
	}
	destroy_shell_env(env);
	free(parser);
	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_memset(&shell, 0, sizeof(t_minishell));
	if (setup_signal() < 0)
		return (1);
	while (1)
	{
		shell.input = readline("minishell> ");
		if (shell.input == NULL)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (shell.input[0] != '\0')
			add_history(shell.input);
		parse_input(shell.input, envp);
		free(shell.input);
	}
	return (EXIT_SUCCESS);
}
