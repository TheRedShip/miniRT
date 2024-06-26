/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 23:18:56 by marvin            #+#    #+#             */
/*   Updated: 2024/05/07 23:18:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H

# include "minirt.h"

# define MAX_RECURSIVE 300

typedef struct s_ray	t_ray;

typedef struct s_boundary
{
	t_vec3f		origin;
	t_vec3f		size;
}				t_boundary;

typedef struct s_kd_tree
{
	int		is_bvh;
	int		has_seen;
	int		max_objects;
}				t_kd_tree;

typedef struct s_bvh
{
	int				leaf;
	int				divided;
	int				obj_count;
	t_boundary		boundary;
	struct s_bvh	*children[2];
	t_list			*objects;
}				t_bvh;

t_boundary	get_boundary(t_objects *object);

int			boundary_intersect(t_boundary boundary, t_objects *object);
int			box_intersection(t_ray ray, t_vec3f origin, t_vec3f rad);

void		create_bvh(t_scene *scene);

#endif