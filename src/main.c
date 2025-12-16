/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/14 10:36:40 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "print.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	parse_input(char *input, char *envp[])
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
		free(lexer);
		return ;
	}
	ast = parsing(parser, envp);
	free_ast(ast);
	free(lexer);
	free(parser);
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
		parse_input(shell.input, envp);
		add_history(shell.input);
	}
	return (EXIT_SUCCESS);
}
