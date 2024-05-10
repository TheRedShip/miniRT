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

t_texture	init_texture(t_scene *scene, char *path)
{
	t_texture	tex;

	tex.exist = 1;
	tex.path = path;
	if (tex.path[ft_strlen(tex.path) - 2] == '\r')
		tex.path[ft_strlen(tex.path) - 2] = '\0';
	if (tex.path[ft_strlen(tex.path) - 1] == '\n')
		tex.path[ft_strlen(tex.path) - 1] = '\0';
	tex.data.img = mlx_xpm_file_to_image(scene->mlx->mlx, tex.path, \
						&tex.width, &tex.height);
	if (tex.data.img == NULL)
	{
		printf("Error: Texture not found : %s/%s\n", getenv("PWD"), path);
		rt_free_scene(scene, 1);
	}
	tex.data.addr = mlx_get_data_addr(tex.data.img, &tex.data.bits_per_pixel, \
									&tex.data.line_length, &tex.data.endian);
	return (tex);
}

t_material	init_material(void)
{
	t_material	mat;

	mat.color = (t_vec3f){0.0f, 0.0f, 0.0f};
	mat.type = MAT_LAMBERT;
	mat.checkered = 0;
	mat.roughness = 1.0f;
	mat.texture.exist = 0;
	mat.specular_probs = 0.0f;
	mat.emission_power = 0.0f;
	mat.refraction_index = 1.0f;
	return (mat);
}

t_objects	*rt_add_objects(t_scene *scene, char *type)
{
	int			i;
	t_objects	*objects;
	static char	*type_list_str[8] = {"sp", "pl", "cy", "qd", "el", "po", "tr", NULL};

	objects = ft_calloc(1, sizeof(t_objects));
	if (!objects)
		rt_free_scene(scene, 1);
	i = -1;
	while (type_list_str[++i] != NULL && ft_strncmp(type, type_list_str[i], 2))
		;
	*objects = (t_objects){.type = i, .material = init_material()};
	if (!scene->objects)
		scene->objects = objects;
	else
		scene->last_objects->next = objects;
	scene->last_objects = objects;
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
	else if (i == OBJ_PORTA)
		objects->portal = ft_calloc(1, sizeof(t_portal));
	else if (i == OBJ_TRIAN)
		objects->triangle = ft_calloc(1, sizeof(t_triangle));
	return (objects);
}

