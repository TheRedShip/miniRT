/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 17:23:35 by marvin            #+#    #+#             */
/*   Updated: 2024/03/31 21:42:14 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_material	init_material()
{
	t_material mat;

	mat.color = (t_vec3f){0.0f, 0.0f, 0.0f};
	mat.type = MAT_LAMBERT;
	mat.checkered = 0;
	mat.roughness = 1.0f;
	mat.specular_probs = 0.0f;
	mat.emission_power = 0.0f;
	mat.refraction_index = 1.0f;
	return (mat);
}

void	rt_set_objects_to_scene(t_scene *scene, t_objects *obj)
{
	t_objects *tmp;

	if (!scene->objects)
		scene->objects = obj;
	else
	{
		tmp = scene->objects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = obj;
	}
}

t_objects	*rt_add_objects(t_scene **scene, char *type)
{
	int	i;
	t_objects *objects;
	static	char *type_list_str[7] = {"sp", "pl", "cy", "qd", "el", "po", NULL};

	objects = ft_calloc(1, sizeof(t_objects));
	if (!objects)
		rt_free_scene(*scene);
	i = -1;
	while (type_list_str[++i] != NULL)
	{
		if (!ft_strncmp(type, type_list_str[i], 2))
			break ;
	}
	objects->type = i;
	objects->material = init_material();
	rt_set_objects_to_scene(*scene, objects);
	if (i == OBJ_SPHER)
		objects->sphere = ft_calloc(1, sizeof(t_sphere));
	else if (i == OBJ_PLANE)
		objects->plane = ft_calloc(1, sizeof(t_plane));
	else if (i == OBJ_CYLIN)
		objects->cylinder = ft_calloc(1, sizeof(t_cylinder));
	else if (i == OBJ_QUADS)
		objects->quad = ft_calloc(1, sizeof(t_quad));
	else if (i == OBJ_ELLIP)
		objects->ellipse = ft_calloc(1, sizeof(t_ellipse));
	else if (i == OBJ_PORTAL)
		objects->portal = ft_calloc(1, sizeof(t_portal));
	return (objects);
}

t_objects	*rt_objlast(t_objects *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
