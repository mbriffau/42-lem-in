/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbriffau <mbriffau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 18:24:15 by mbriffau          #+#    #+#             */
/*   Updated: 2018/03/03 18:55:34 by mbriffau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lem_in.h"

char	*save_line(char *map, char *s, int *f)
{
	if (*f & FIRST_LINE)
	{
		if (!(map = ft_strnjoinfree(map, "\n", 1, 'L')))
			exit(1);
	}
	if (!(map = ft_strnjoinfree(map, s, ft_strlen(s), 'B')))
		exit(1);
	*f += ((*f & FIRST_LINE) ? 0 : FIRST_LINE);
	return (map);
}

t_lem	*parse_data(t_lem *l, char *s)
{
	int name_size;

	name_size = 0;
	if (!(l->f & 0x13F) && isvalid_ant_number(s) == 1)
		check_ant(&*l, s);
	else if (((l->f & 0x6) != 0x6) && isvalid_command(s) && (!(l->f & 24)))
		parse_order(&*l, s);
	else if (isvalid_comment(s))
		;
	else if (((l->f & PIPE) == 0) && (name_size = isvalid_room(s)))
	{
		parse_room(&*l, s, name_size);
		(l->f & ROOM) == 0 ? l->f += ROOM : 0;
	}
	else if ((l->f & 0x106) && isvalid_pipe(s) && (!(l->f & 24)))
	{
		((l->f & PIPE) == 0) ? malloc_ptr_pipe(&*l) : 0;
		(l->f & PIPE) == 0 ? l->f += PIPE : 0;
		parse_pipe(&*l, s);
	}
	else
		ft_error("ERROR");
	return (l);
}

void	get_data(t_lem *l)
{
	char	*s;

	if (!(l->map = ft_strndup("\0", 1)))
		exit(1);
	while (get_next_line(0, &s) > 0)
	{
		if (((l->f & DATA) == 0) && (*s == 0))
			l->f += DATA;
		if ((l->f & DATA) == 0)
			l = parse_data(l, s);
		l->map = save_line(l->map, s, &l->f);
	}
	if ((l->f & 423) != 423)
		ft_error("ERROR");
	l->lmap = ft_strsplit(l->map, '\n');
}

int		main(void)
{
	t_lem	l;

	ft_bzero(&l, sizeof(t_lem));
	l.room_list = NULL;
	get_data(&l);
	algo(&l);
	ft_printf("%s\n\n", l.map);
	print_ant(&l);
	return (0);
}
