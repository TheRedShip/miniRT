/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 19:12:27 by marvin            #+#    #+#             */
/*   Updated: 2024/03/28 19:12:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <fcntl.h>

# include "minirt.h"

void		rt_parse(char *file, t_scene **scene);

int			rt_return(char **split);
int			rt_range_atoi(char *line, int min, int max);
float		rt_range_atof(char *line, float min, float max);
t_veci3		rt_atoi3(char *line, int min, int max);
t_vecf3		rt_atof3(char *line, float min, float max);

int			rt_verify_camera(char *line);
int			rt_verify_ambient(char *line);
int			rt_verify_light(char *line);
int			rt_verify_sphere(char *line);
int			rt_verify_plane(char *line);
int			rt_verify_cylinder(char *line);

int			rt_parse_ambient(char *line, t_scene **scene);
int			rt_parse_camera(char *line, t_scene **scene);
int			rt_parse_light(char *line, t_scene **scene);
int			rt_parse_sphere(char *line, t_scene **scene);
int			rt_parse_plane(char *line, t_scene **scene);
int			rt_parse_cylinder(char *line, t_scene **scene);

#endif