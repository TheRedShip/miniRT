/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:28:05 by marvin            #+#    #+#             */
/*   Updated: 2024/03/31 00:28:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3f	vec3f_add_v(t_vec3f a, t_vec3f b)
{
	t_vec3f	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

t_vec3f	vec3f_sub_v(t_vec3f a, t_vec3f b)
{
	t_vec3f	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return (result);
}

t_vec3f	vec3f_mul_v(t_vec3f a, t_vec3f b)
{
	t_vec3f	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

t_vec3f	vec3f_div_v(t_vec3f a, t_vec3f b)
{
	t_vec3f	result;

	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	return (result);
}