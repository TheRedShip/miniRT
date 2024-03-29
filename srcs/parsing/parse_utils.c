/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:44:30 by marvin            #+#    #+#             */
/*   Updated: 2024/03/28 22:44:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int		rt_return(char **split)
{
	if (split)
		free(split);
	return (0);
}

int			rt_range_atoi(char *line, int min, int max)
{
	int		n;

	n = ft_atoi(line);
	if (n < min || n > max)
		return (-1);
	return (1);
}

float		rt_range_atof(char *line, float min, float max)
{
	float		n;

	n = ft_atof(line, 1);
	if (n < min || n > max)
		return (-1.0);
	return (1.0);

}

t_veci3		rt_atoi3(char *line, int min, int max)
{
	t_veci3		vec;
	char		**split;

	split = ft_split(line, ',');
	if (ft_tab_len(split) != 3)
	{
		free(split);
		return ((t_veci3){-1, -1, -1});
	}
	vec.x = ft_atoi(split[0]);
	vec.y = ft_atoi(split[1]);
	vec.z = ft_atoi(split[2]);
	free(split);
	if (vec.x < min || vec.x > max || vec.y < min || \
		vec.y > max || vec.z < min || vec.z > max)
		return ((t_veci3){-1, -1, -1});
	return (vec);
}

t_vecf3		rt_atof3(char *line, float min, float max)
{
	t_vecf3		vec;
	char		**split;

	split = ft_split(line, ',');
	if (ft_tab_len(split) != 3)
	{
		free(split);
		return ((t_vecf3){-1, -1, -1});
	}
	vec.x = ft_atof(split[0], 1);
	vec.y = ft_atof(split[1], 1);
	vec.z = ft_atof(split[2], 1);
	free(split);
	if (vec.x < min || vec.x > max || vec.y < min || \
		vec.y > max || vec.z < min || vec.z > max)
		return ((t_vecf3){-1, -1, -1});
	return (vec);
}