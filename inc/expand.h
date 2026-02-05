/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:02:24 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "eval.h"
# include <dirent.h>
# include <sys/types.h>
# include <unistd.h>

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}						t_quote_state;

typedef struct s_expand_ctx
{
	t_shell_env			*env;
	t_quote_state		quote_state;
	int					last_exit_code;
	pid_t				shell_pid;
}						t_expand_ctx;

typedef struct s_match_list
{
	char				*match;
	struct s_match_list	*next;
}						t_match_list;

char					**expand_args(char **args, t_shell_env *env);
char					*expand_string(const char *str, t_expand_ctx *ctx);

char					*expand_variables(const char *str, t_expand_ctx *ctx);
char					*expand_var_name(const char *str, int *len,
							t_expand_ctx *ctx);
char					*get_var_value(const char *name, t_expand_ctx *ctx);
int						is_valid_var_char(char c, int is_first);

char					*expand_exit_status(t_expand_ctx *ctx);
char					*expand_shell_pid(t_expand_ctx *ctx);
char					*expand_shell_name(t_expand_ctx *ctx);

char					*remove_quotes(const char *str);
t_quote_state			update_quote_state(char c, t_quote_state current);
int						is_in_single_quotes(t_quote_state state);
int						is_in_double_quotes(t_quote_state state);

char					**expand_wildcards(const char *pattern);
int						has_wildcard(const char *str);
int						match_pattern(const char *pattern, const char *str);
t_match_list			*get_matching_files(const char *dir,
							const char *pattern);
void					free_match_list(t_match_list *list);
char					**match_list_to_array(t_match_list *list);

int						count_matches(t_match_list *list);
void					sort_matches(char **matches, int count);
t_match_list			*add_match(t_match_list *list, const char *match);
char					*extract_directory(const char *pattern);
char					*extract_pattern(const char *pattern);

t_expand_ctx			*create_expand_ctx(t_shell_env *env);
void					destroy_expand_ctx(t_expand_ctx *ctx);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_charjoin(char *s, char c);

#endif
