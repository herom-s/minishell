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
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "minishell_signal.h"
#include <unistd.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

int	create_lexer_parser(char *input, t_lexer **lexer, t_parser **parser)
{
	*lexer = create_lexer(input);
	if (lexer == NULL)
	{
		ft_printf("Failed to create lexer\n");
		return (-1);
	}
	*parser = create_parser(*lexer);
	if (parser == NULL)
	{
		ft_printf("Failed to create parser\n");
		free(*lexer);
		return (-1);
	}
	return (0);
}

void	parse_input(char *input, char *envp[])
{
	t_shell_env			*env;
	t_shell_response	*res;
	t_lexer				*lexer;
	t_parser			*parser;
	t_ast				*ast;

	res = NULL;
	parser = NULL;
	lexer = NULL;
	if (create_lexer_parser(input, &lexer, &parser) < 0)
		return ;
	env = create_shell_env(envp);
	ast = init_ast(parser);
	if (ast)
	{
		res = eval_ast(ast, env);
		if (res)
			free(res);
		free_ast(ast);
	}
	destroy_shell_env(env);
	free(parser);
	ft_lstclear(&(lexer->tokens), &free_token);
	free(lexer);
}

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

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;
	int			status;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_minishell));
	setup_nonfork_signal();
	while (1)
	{
		g_sig = 0;
		status = get_input(&shell);
		if (status == 1)
			continue ;
		if (status == -1)
			break ;
		parse_input(shell.input, envp);
		free(shell.input);
	}
	return (EXIT_SUCCESS);
}
