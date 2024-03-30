/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:19:39 by marvin            #+#    #+#             */
/*   Updated: 2024/03/30 14:19:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

#include "minirt.h"

void	create_window(t_scene **scene);
void	put_pixel(t_scene *scene, int x, int y, int color);

int		key_hook(int keycode, t_scene *scene);
int		mouse_hook(int button, int x, int y, t_scene *scene);
#endif