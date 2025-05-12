/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 00:52:26 by yaamaich          #+#    #+#             */
/*   Updated: 2025/04/25 12:18:15 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_list
{
	int				content;
	int				index;
	struct s_list	*next;
}					t_list;

void				ft_exit_error(void);
void				*ft_calloc(size_t count, size_t size);
void				*ft_bzero(void *s, size_t n);
int					ft_isprint(int c);
void				*ft_memcpy(void *to, const void *from, size_t c);
void				*ft_memmove(void *to, const void *from, size_t len);
void				*ft_memset(void *str, int c, size_t len);
size_t				ft_strlen(const char *c);
int					ft_isalnum(int str);
int					ft_isalpha(int str);
int					ft_isascii(int a);
int					ft_isdigit(int str);
int					ft_toupper(int c);
int					ft_tolower(int c);
char				*ft_strchr(const char *str, int ch);
char				*ft_strrchr(const char *s, int c);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
void				*ft_memchr(const void *str, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
char				*ft_strnstr(const char *str1, const char *str2, size_t len);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
int					ft_atoi(const char *str);
char				*ft_strdup(const char *s1);
size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
void				ft_putstr_fd(char *str, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putendl_fd(char *s, int fd);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_itoa(int n);
char				**ft_split(char const *s, char c);
t_list				*ft_lstnew(int content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
// void				ft_lstdelone(t_list *lst, void (*del)(int *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
int					ft_sqrt(int nb);
char				*get_next_line(int fd);
int					ft_putnbr_max(long n, int base, char type);
int					ft_putunsnbr(unsigned int n);
int					ft_putptr(void *s);
int					ft_putchar(char c);
int					ft_putstr(char *str);
int					ft_printf(const char *format, ...);
int					ft_strcmp(char*s1, char	*s2);

#endif
