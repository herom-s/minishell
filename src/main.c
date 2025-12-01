/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/01 06:25:07 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "libft.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include "print.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	parse_input(char *input)
{
	t_lexer		*lexer;
	t_parser	*parser;
	t_ast		*ast;

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
		free_lexer(lexer);
		return ;
	}
	ast = parsing(parser);
	free_lexer(lexer);
	free_parser(parser);
}

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
		if (ft_strncmp(shell.input, "exit", 4) == 0)
			break ;
		parse_input(shell.input);
		add_history(shell.input);
//		shell.ast_shell = read_input(shell.input);
//		shell.shell_response = eval_ast(shell.ast_shell);
//		print_shell_response(shell.shell_response);
		free(shell.input);
	}
	return (EXIT_SUCCESS);
}
