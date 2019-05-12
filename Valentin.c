/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Valentin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctestabu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 14:51:55 by ctestabu          #+#    #+#             */
/*   Updated: 2019/05/12 14:51:55 by ctestabu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int		ft_push_list(t_tetr **head, char **str)
{
	t_tetr *tmp;
	if (!(tmp = malloc(sizeof(t_tetr))))
		return (0);
	tmp->next = *head;
	tmp->str = str;
	*head = tmp;
	return (1);
}

int		ft_top(char *str)
{
	int i;

	i = 0;
	if (str[0] == '.' && str[1] == '.'
			&& str[2] == '.' && str[3] == '.')
	{
		while (str[i])
		{
			if (str[i] == '#')
			{
				str[i] = '.';
				str[i - 4] = '#';
			}
			i++;
			if (i == 16 && str[0] == '.' && str[1] == '.'
					&& str[2] == '.' && str[3] == '.')
				i = 0;
		}
	}
	return (0);
}


int		ft_left(char *str)
{
	int i;

	i = 0;
	if (str[0] == '.' && str[4] == '.'
			&& str[8] == '.' && str[12] == '.')
	{
		while (str[i])
		{
			if (str[i] == '#')
			{
				str[i] = '.';
				str[i - 1] = '#';
			}
			i++;
			if (i == 16 && str[0] == '.' && str[4] == '.'
					&& str[8] == '.' && str[12] == '.')
				i = 0;
		}
	}
	return (0);
}

static int  fill_str(char *line, char **str)
{
	if (*str == NULL)
	{
		if (!(*str = ft_strdup(line)))
			return (0);
	}
	else
	{
		if (!(*str = ft_strjoin(*str, line)))
			return (0);
	}
	return (1);
}

void	ft_print_str(char *str)
{
	int j;

	j = 0;
	printf("%s", str);
//	while (str[j])
//	{
//		printf("%c", str[j]);
//		if (j == 3 || j == 7 || j == 11 || j == 15)
//			printf("\n");
//		j++;
//	}
	printf("\n");
}

void	ft_print_list(t_tetr *begin_list)
{
	int j;

	j = 0;
	t_tetr *ptr;
	ptr = begin_list;
	while (ptr)
	{
		while (j < 4)
		{
			printf("%s\n", ptr->str[j]);
			j++;
		}
		printf("\n");
		j = 0;
		ptr = ptr->next;
	}
}

char	**ft_to_matrix(char *str)
{
	int i;
	int j;
	int k;
	char **mtx_str;

	i = 0;
	j = 0;
	k = 0;
	if (!(mtx_str = (char **)malloc(sizeof(char *) * 4)))
		return (NULL);
	while (i < 4)
	{
		if (!(mtx_str[i] = (char *)malloc(sizeof(char) * 5)))
			return (NULL);
		while (j < 4)
			mtx_str[i][j++] = str[k++];
		mtx_str[i][j] = '\0';
		i++;
		j = 0;
	}
	return (mtx_str);
}

static int find_tetro_width(char **mtx_str) // ширина фигуры
{
	int width;
	int i;
	int j;

	width = 0;
	j = 0;
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			if (mtx_str[i][j] == '#')
			{
				width++;
				i = 3;
			}
			i++;
		}
		j++;
	}
	return (width);
}

static int find_tetro_height(char **mtx_str) // высота фигуры
{
	int height;
	int i;
	int j;

	height = 0;
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (mtx_str[i][j] == '#')
			{
				height++;
				j = 3;
			}
			j++;
		}
		i++;
	}
	return (height);
}

static void	to_letters(char **str_arr)
{
	static char	letter = 'A';
	int			i;
	int			j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (str_arr[i][j] == '#')
				str_arr[i][j] = letter;
			j++;
		}
		i++;
	}
	letter++;
}

static int	check_tetro(char *str)
{
	int	i;
	int	hash;
	int	point;

	i = 0;
	hash = 0;
	point = 0;
	while (str[i])
	{
		if (str[i] == '#')
			hash++;
		if (str[i] == '.')
			point++;
		i++;
	}
	if (hash != 4 || point != 12 || i != 16)
		return (0);
	return (1);
}

static int	check_connections(char *str)
{
	int	con;
	int	i;

	con = 0;
	i = 0;
	while (i < 16)
	{
		if (str[i] == '#')
		{
			if ((i + 1) < 16 && str[i + 1] == '#')
				con++;
			if ((i - 1) >= 0 && str[i - 1] == '#')
				con++;
			if ((i + 4) < 16 && str[i + 4] == '#')
				con++;
			if ((i - 4) >= 0 && str[i - 4] == '#')
				con++;
		}
		i++;
	}
	if (con != 6 && con != 8)
		return (0);
	return (1);
}
static int	checks(char *line, char **str, int *i)
{
	static int	tetro = 0;

	if (ft_strlen(line) || check_tetro(*str) != 1 || check_connections(*str) != 1)
		return (0);
	tetro = tetro + 1;
	if (tetro > 26)
		return (0);
	*i = 1;
	return (1);
}

static int	ft_fillit(int fd)
{
	int		i;
	int     ret;
	char    *str;
	char    *line;
	char	**mtx_str;
	t_tetr	*begin_list;

	i = 1;
	ret = 1;
	str = NULL;
	line = NULL;
	mtx_str = NULL;
	begin_list = NULL;
	while ((ret = get_next_line(fd, &line)) >= 0)
	{
		if (ret != 0  && !(fill_str(line, &str)))
			return (0);
		if (i++ % 5 == 0)
		{
			ft_left(str);
			ft_top(str);
//			check_tetro(str);
//			check_connections(str);
			checks(line, &str, &i);
			mtx_str = ft_to_matrix(str);
//			ft_print_str(str);
			ft_push_list(&begin_list, mtx_str);
			ft_strdel(&str);
			if (ret == 0)
				break;
		}

	}
	ft_print_list(begin_list);
	return (1);
}



int 	main(void)
{
	int fd;

	if (!(fd = open("valid_tet.txt", O_RDONLY)))
//	if (!(fd = open("text.txt", O_RDONLY)))
	{
		printf("error");
		return (0);
	}
	if (!(ft_fillit(fd)))
		return (0);
	return (0);
}