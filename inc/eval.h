/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thaperei <thaperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:21:40 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/05 14:56:57 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "ast.h"
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

typedef t_cmd_response	*(*t_cmd_func)(t_ast *shell_ast, char **cmd_str,
							char *envp[]);

typedef struct s_cmd_func_call
{
	t_cmd_func			cmd_func;
	char				**cmd_str;
	char				**envp;
}						t_cmd_func_call;

t_shell_response		*eval_ast(t_ast *shell_ast, char *envp[]);

char					**get_cmd_str(char *cmd_name, t_ast *cmd_suffix,
							char *envp[]);
void					free_cmd_str(char **cmd_str);

char					**get_bin_paths(char *envp[]);
int						check_path(char *path, char *argv);
int						check_builtin(char *cmd_name);

t_cmd_response			*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_cd(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_echo(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_env(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_export(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_pwd(t_ast *shell_ast, char **cmd_str,
							char *envp[]);
t_cmd_response			*func_built_in_unset(t_ast *shell_ast, char **cmd_str,
							char *envp[]);

char					*get_curdir(void);
size_t					num_arguments(char **cmd_str);

#endif
