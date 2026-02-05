/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 08:26:02 by thaperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <stdlib.h>

static int	has_unclosed_quotes(const char *str,
		int is_dquotes_open, int is_squotes_open)
{
	while (*str != '\0')
	{
		if (is_dquotes_open)
		{
			if (*str == '"')
				is_dquotes_open = 0;
		}
		else if (is_squotes_open)
		{
			if (*str == '\'')
				is_squotes_open = 0;
		}
		else
		{
			if (*str == '"')
				is_dquotes_open = 1;
			else if (*str == '\'')
				is_squotes_open = 1;
		}
		str++;
	}
	return (is_dquotes_open || is_squotes_open);
}

static int	create_lexer_parser(char *input, t_shell_env *env)
{
	if (has_unclosed_quotes(input, 0, 0))
	{
		ft_printf("syntax error: unclosed quotes\n");
		return (-1);
	}
	env->lexer = create_lexer(input);
	if (env->lexer == NULL)
	{
		ft_printf("Failed to create lexer\n");
		return (-1);
	}
	env->parser = create_parser(env->lexer);
	if (env->parser == NULL)
	{
		ft_printf("Failed to create parser\n");
		free(env->lexer);
		return (-1);
	}
	return (0);
}

static void	cleanup_lexer_parser(t_shell_env *env)
{
	if (env->parser)
	{
		free(env->parser);
		env->parser = NULL;
	}
	if (env->lexer)
	{
		ft_lstclear(&(env->lexer->tokens), &free_token);
		free(env->lexer);
		env->lexer = NULL;
	}
}

void	parse_input(char *input, t_shell_env *env)
{
	t_shell_response	*res;
	t_ast				*ast;

	res = NULL;
	if (create_lexer_parser(input, env) < 0)
		return ;
	ast = init_ast(env->parser);
	if (ast)
	{
		env->root_node = ast;
		res = eval_ast(ast, env);
		if (res)
			free(res);
		free_ast(ast);
		env->root_node = NULL;
	}
	cleanup_lexer_parser(env);
}
