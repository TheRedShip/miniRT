/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_structures.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:14:33 by marvin            #+#    #+#             */
/*   Updated: 2024/04/09 00:08:10 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_STRUCTURES_H
# define SCENE_STRUCTURES_H

# include "minirt.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_mlx
{
	void	*mlx;
	void	*win;
	int		is_acc;
	int		frame_index;
	int		antialiasing;
	t_data	img;
	t_vec3f	**acc_img;
}				t_mlx;

typedef struct	s_mouse
{
	t_vec2f pos;
	int		is_pressed;
}				t_mouse;

typedef struct	s_scene
{
	t_ambient_light	*ambient_light;
	t_camera		*camera;
	t_light			*lights;
	t_objects		*objects;
	t_mlx			*mlx;
	t_mouse			mouse;
}				t_scene;

typedef struct s_threads
{
	int			id;
	pthread_t	thread;
	t_scene		*scene;
}				t_threads;

#endif
