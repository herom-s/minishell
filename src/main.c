/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:13:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/22 14:32:17 by thaperei         ###   ########.fr       */
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
	t_token		*token;

	lexer = create_lexer(input);
	while (1)
	{
		token = get_next_token(lexer);
		if (token->type == END)
			break ;
		printf("%s - %d\n", token->literal, token->type);
		free(token->literal);
		free(token);
	}
	free(lexer);
	free(input);
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
		parse_input(shell.input);
		add_history(shell.input);
//		shell.ast_shell = read_input(shell.input);
//		shell.shell_response = eval_ast(shell.ast_shell);
//		print_shell_response(shell.shell_response);
	}
	return (EXIT_SUCCESS);
}
