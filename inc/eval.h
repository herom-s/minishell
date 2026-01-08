/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:21:40 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/08 16:19:15 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "ast.h"
# include "hashtable.h"
# include <stdlib.h>

# define INPUT_END 0
# define OUTPUT_END 1
# define PIPE_OUT 0
# define PIPE_ERR 1

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
	char				**envp;
	t_hashtable			*vars;
	t_list				*order;
	t_ast				*root_node;
}						t_shell_env;

typedef t_cmd_response	*(*t_cmd_func)(t_ast *shell_ast, char **cmd_str,
							t_shell_env *shell_env);

typedef struct s_cmd_func_call
{
	int					is_builtin;
	t_cmd_func			cmd_func;
	t_shell_env			*env;
	char				**cmd_str;
	char				**envp;
}						t_cmd_func_call;

t_shell_response		*eval_ast(t_ast *shell_ast, t_shell_env *env);

char					**get_cmd_str(char *cmd_name, t_ast *cmd_suffix,
							char *envp[]);
void					free_cmd_str(char **cmd_str);

char					**get_bin_paths(char *envp[]);
int						check_path(char *path, char *argv);
int						check_builtin(char *cmd_name);

void					read_stream(int fd, char **str);

t_cmd_response			*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);
t_cmd_response			*func_built_in_cd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);
t_cmd_response			*func_built_in_echo(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);
t_cmd_response			*func_built_in_env(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);
t_cmd_response			*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);

char					*build_plain_env_line(char *key, char *value);
void					append_env_plain_line(char **output, char *key,
							t_shell_env *env);
char					*build_env_line(char *key, char *value);
void					append_env_line(char **output, char *key,
							t_shell_env *env);
int						process_export_arg(char *arg, t_shell_env *env,
							char **error);
t_cmd_response			*func_built_in_export(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);

t_cmd_response			*func_built_in_pwd(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);
t_cmd_response			*func_built_in_unset(t_ast *shell_ast, char **cmd_str,
							t_shell_env *env);

t_shell_env				*create_shell_env(char *envp[]);
void					*destroy_shell_env(t_shell_env *env);

t_cmd_response			*create_cmd_res(void);
void					*destroy_cmd_res(t_cmd_response *cmd_res);

void					create_child_fds(int *fd_out, int *fd_err);
void					child_exit(t_shell_env *env, t_ast *local_ast,
							int code);

typedef struct s_pipe_context
{
	int	*writer;
	int	*reader;
	int	prev_pipe_fd_read_end;
}						t_pipe_context;

t_cmd_response			*handle_parent(pid_t pid, int *fd_out, int *fd_err);

void					create_pipes_or_fail(int *fd_out, int *fd_err);
void					*closes_pipes(int *fd_out, int *fd_err);

void					func_exec_cmd_pipe(t_ast *ast, t_shell_env *env,
							int *pipe_fd, int prev_pipe_read_fd);
void					eval_pipe_recursive(t_ast *shell_ast, t_shell_env *env,
							int *pipefd, int prev_read_end);

int						has_redirections(t_ast *shell_ast);

int						eval_redir(t_ast *shell_ast);

char					*get_curdir(void);
t_cmd_func				get_cmd_func(char *cmd_name);
t_cmd_func_call			*check_cmd(t_ast *shell_ast, t_shell_env *env);
size_t					num_arguments(char **cmd_str);

#endif
