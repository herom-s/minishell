/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:21:40 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/16 14:25:31 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "ast.h"
# include "hashtable.h"
# include <stdlib.h>

typedef enum s_cmd_type
{
	CMD_CD,
	CMD_PWD,
	CMD_ENV,
	CMD_EXPORT,
	CMD_UNSET,
	CMD_ECHO,
	CMD_EXIT,
}						t_cmd_type;

typedef struct s_shell_response
{
	char				*output;
	char				*curr_dir;
	int					exit_code;
	char				*erro_msg;
}						t_shell_response;

typedef struct s_cmd_response
{
	char				*output;
	char				*curr_dir;
	int					exit_code;
	char				*erro_msg;
}						t_cmd_response;

typedef struct s_shell_env
{
	t_hashtable			*vars;
	t_list				*order;
}						t_shell_env;

typedef t_cmd_response	*(*t_cmd_func)(t_ast *shell_ast, char **cmd_str,
							t_shell_env *shell_env, char *envp[]);

typedef struct s_cmd_func_call
{
	t_cmd_func			cmd_func;
	t_shell_env			*env;
	char				**cmd_str;
	char				**envp;
}						t_cmd_func_call;

t_shell_response		*eval_ast(t_ast *shell_ast, t_shell_env *env,
							char *envp[]);

char					**get_cmd_str(char *cmd_name, t_ast *cmd_suffix,
							char *envp[]);
void					free_cmd_str(char **cmd_str);

char					**get_bin_paths(char *envp[]);
int						check_path(char *path, char *argv);
int						check_builtin(char *cmd_name);

t_cmd_response			*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);
t_cmd_response			*func_built_in_cd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);
t_cmd_response			*func_built_in_echo(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);
t_cmd_response			*func_built_in_env(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);
t_cmd_response			*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);

char					*build_env_line(char *key, char *value);
void					append_env_line(char **output, char *key,
							t_shell_env *env);
int						process_export_arg(char *arg, t_shell_env *env,
							char **error);
t_cmd_response			*func_built_in_export(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);

t_cmd_response			*func_built_in_pwd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);
t_cmd_response			*func_built_in_unset(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env, char *envp[]);

t_shell_env				*create_shell_env(char *envp[]);
void					*destroy_shell_env(t_shell_env *env);

char					*get_curdir(void);
t_cmd_func				get_cmd_func(char *cmd_name);
size_t					num_arguments(char **cmd_str);

#endif
