/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:46:29 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:04:39 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "hashtable.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static void	parse_env_var(t_shell_env *env, char *env_str)
{
	char	*tmp_var;
	char	*after_equal;

	tmp_var = ft_strdup(env_str);
	after_equal = ft_strchr(tmp_var, '=');
	*after_equal = '\0';
	hashtable_set(env->vars, tmp_var, ft_strdup(after_equal + 1));
	ft_lstadd_back(&env->order, ft_lstnew(ft_strdup(tmp_var)));
	free(tmp_var);
}

t_shell_env	*create_shell_env(char *envp[])
{
	t_shell_env	*env;
	size_t		i;

	env = ft_calloc(1, sizeof(t_shell_env));
	if (!env)
		return (NULL);
	env->og_stdin_fd = -1;
	env->og_stdout_fd = -1;
	if (isatty(STDIN_FILENO))
		env->interactive_owner = 1;
	env->vars = hashtable_create();
	if (!env->vars)
		return (free(env), NULL);
	env->envp = envp;
	i = 0;
	while (envp[i])
	{
		parse_env_var(env, envp[i]);
		i++;
	}
	return (env);
}

static void	destroy_env_resources(t_shell_env *env)
{
	if (env->current_input)
	{
		free(env->current_input);
		env->current_input = NULL;
	}
	if (env->current_call)
	{
		free_cmd_str(env->current_call->cmd_str);
		env->current_call->cmd_str = NULL;
		free(env->current_call);
		env->current_call = NULL;
	}
}

static void	destroy_env_parser(t_shell_env *env)
{
	if (env->root_node)
	{
		free_heredoc_filenames(env->root_node);
		free_ast(env->root_node);
		env->root_node = NULL;
	}
	if (env->parser)
	{
		free(env->parser);
		env->parser = NULL;
	}
	if (env->lexer)
	{
		if (env->lexer->tokens)
			ft_lstclear(&(env->lexer->tokens), &free_token);
		free(env->lexer);
		env->lexer = NULL;
	}
}

void	*destroy_shell_env(t_shell_env *env)
{
	if (!env)
		return (NULL);
	if (env->og_stdin_fd >= 0)
		close(env->og_stdin_fd);
	if (env->og_stdout_fd >= 0)
		close(env->og_stdout_fd);
	destroy_env_resources(env);
	destroy_env_parser(env);
	if (env->vars)
	{
		hashtable_destroy(env->vars);
		env->vars = NULL;
	}
	if (env->order)
	{
		ft_lstclear(&env->order, free);
		env->order = NULL;
	}
	rl_clear_history();
	free(env);
	return (NULL);
}
