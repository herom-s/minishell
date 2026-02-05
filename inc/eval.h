/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:21:40 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 16:38:17 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "ast.h"
# include "hashtable.h"
# include "lexer.h"
# include "parser.h"
# include <stdlib.h>
# include <termios.h>

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
}								t_cmd_type;

typedef struct s_shell_response	t_shell_response;
typedef struct s_cmd_response	t_cmd_response;
typedef struct s_shell_env		t_shell_env;
typedef struct s_cmd_func_call	t_cmd_func_call;

typedef struct s_shell_response
{
	int							exit_code;
}								t_shell_response;

typedef struct s_cmd_response
{
	int							exit_code;
}								t_cmd_response;

typedef struct s_shell_env
{
	t_cmd_func_call				*current_call;
	int							interactive_owner;
	int							last_exit_code;
	int							og_stdout_fd;
	int							og_stdin_fd;
	char						*current_input;
	char						**envp;
	t_hashtable					*vars;
	t_list						*order;
	t_lexer						*lexer;
	t_parser					*parser;
	t_ast						*root_node;
	struct termios				saved_termios;
	int							termios_saved;
}								t_shell_env;

typedef t_cmd_response			*(*t_cmd_func)(t_ast *shell_ast, char **cmd_str,
									t_shell_env *shell_env);

typedef struct s_cmd_func_call
{
	int							is_builtin;
	t_cmd_func					cmd_func;
	t_shell_env					*env;
	char						**cmd_str;
	char						**envp;
}								t_cmd_func_call;

t_shell_response				*eval_ast(t_ast *shell_ast, t_shell_env *env);

char							**get_cmd_str(char *cmd_name, t_ast *cmd_suffix,
									t_shell_env *env);
void							free_cmd_str(char **cmd_str);
int								count_cmd_suffix(t_ast *cmd_suffix);

char							**get_bin_paths(t_shell_env *env);
int								check_path(char *path, char *argv);
char							*resolve_command_path(char *cmd_name,
									t_shell_env *env);
int								check_builtin(char *cmd_name);

t_cmd_response					*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
									t_shell_env *env);
t_cmd_response					*func_built_in_cd(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);
t_cmd_response					*func_built_in_echo(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);
t_cmd_response					*func_built_in_env(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);
t_cmd_response					*func_built_in_exit(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);

char							*build_plain_env_line(char *key, char *value);
void							append_env_plain_line(char **output, char *key,
									t_shell_env *env);
char							*build_env_line(char *key, char *value);
void							append_env_line(char **output, char *key,
									t_shell_env *env);
int								process_export_arg(char *arg, t_shell_env *env,
									char **error);
t_cmd_response					*func_built_in_export(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);
char							*export_no_args(t_shell_env *env);

t_cmd_response					*func_built_in_pwd(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);
t_cmd_response					*func_built_in_unset(t_ast *shell_ast,
									char **cmd_str, t_shell_env *env);

t_shell_env						*create_shell_env(char *envp[]);
void							*destroy_shell_env(t_shell_env *env);

t_cmd_response					*create_cmd_res(void);
void							*destroy_cmd_res(t_cmd_response *cmd_res);

void							save_original_std_fds(t_shell_env *env);
void							restore_original_std_fds(t_shell_env *env);
void							child_exit(t_shell_env *env, t_ast *local_ast,
									int code);

typedef struct s_pipe_context
{
	int							*writer;
	int							*reader;
	int							prev_pipe_fd_read_end;
	pid_t						left_pid;
}								t_pipe_context;

t_cmd_response					*handle_parent(pid_t pid);

void							run_pipe_child(t_ast *ast, t_shell_env *env,
									t_pipe_context *ctx);
void							func_exec_cmd_pipe(t_ast *ast, t_shell_env *env,
									int *pipe_fd, int prev_pipe_read_fd);
void							eval_pipe_recursive(t_ast *shell_ast,
									t_shell_env *env, int *pipefd,
									int prev_read_end);

t_cmd_response					*eval_cmd(t_ast *shell_ast, t_shell_env *env);
t_cmd_response					*eval_pipe(t_ast *shell_ast, t_shell_env *env);
t_cmd_response					*eval_node(t_ast *node, t_shell_env *env);
t_cmd_response					*eval_subshell(t_ast *shell_ast,
									t_shell_env *env);
void							eval_subshell_in_pipe(t_ast *shell_ast,
									t_shell_env *env);
t_cmd_response					*eval_and_or(t_ast *shell_ast,
									t_shell_env *env);

t_cmd_response					*eval_external_cmd(t_ast *shell_ast,
									t_cmd_func_call *call);
t_cmd_response					*exec_call(t_ast *ast, t_cmd_func_call *call);

char							*read_line_interactive(void);
char							*read_line_noninteractive(void);
int								should_stop_heredoc(char *line,
									char *delimiter);
char							*generate_heredoc_filename(void);
int								write_heredoc_to_file(char *delimiter,
									char *filename, t_shell_env *env,
									int expand);
int								process_heredocs(t_ast *node, t_shell_env *env);
void							cleanup_heredoc_files(t_ast *ast);
void							free_heredoc_filenames(t_ast *ast);

void							unlink_heredoc_node(t_ast *node);
void							free_heredoc_mem(t_ast *node);
void							cleanup_simple_cmd_heredocs(t_ast *ast);
void							free_simple_cmd_heredocs(t_ast *ast);
void							free_heredoc_prefix(t_ast *prefix);

int								eval_redir(t_ast *shell_ast, t_shell_env *env);
int								eval_io_file(t_ast *io_file, t_shell_env *env);
int								get_fd_for_op(const char *filename,
									t_token_type op);
char							*expand_redir_target(const char *file,
									t_shell_env *env);

char							*get_curdir(void);
t_cmd_func						get_cmd_func(char *cmd_name);
t_cmd_func_call					*check_cmd(t_ast *shell_ast, t_shell_env *env);
size_t							num_arguments(char **cmd_str);

#endif
