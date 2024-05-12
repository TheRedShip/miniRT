/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:34:14 by marvin            #+#    #+#             */
/*   Updated: 2024/04/06 19:34:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_hit_info	hit_sphere(t_ray r, t_objects *obj, t_sphere *sp)
{
	float		a;
	float		b;
	float		c;
	t_hit_info	hit_info;
	float		discriminant;

	hit_info.distance = -1.0f;
	r.origin = v_sub_v(r.origin, obj->origin);
	a = v_dot(r.direction, r.direction);
	b = 2.0f * v_dot(r.origin, r.direction);
	c = v_dot(r.origin, r.origin) - (sp->diameter / 2) * (sp->diameter / 2);
	r.origin = v_add_v(r.origin, obj->origin);
	discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return (hit_info);
	hit_info.distance = (-b - sqrt(discriminant)) / (2.0f * a);
	if (hit_info.distance < 0.0f)
		hit_info.distance = (-b + sqrt(discriminant)) / (2.0f * a);
	hit_info.position = v_add_v(r.origin, \
						v_mul_f(r.direction, hit_info.distance));
	hit_info.normal = normalize(v_sub_v(hit_info.position, obj->origin));
	return (hit_info);
}

t_hit_info	hit_ellipse(t_ray ray, t_objects *obj, t_ellipse *ellipse)
{
	float		a;
	float		b;
	float		c;
	t_hit_info	hit;
	float		discriminant;

	ray.origin = v_sub_v(ray.origin, obj->origin);
	a = ray.direction.x * ray.direction.x / (ellipse->a * ellipse->a) + \
		ray.direction.y * ray.direction.y / (ellipse->b * ellipse->b) + \
		ray.direction.z * ray.direction.z / (ellipse->c * ellipse->c);
	b = 2.0f * ray.origin.x * ray.direction.x / (ellipse->a * ellipse->a) + \
		2.0f * ray.origin.y * ray.direction.y / (ellipse->b * ellipse->b) + \
		2.0f * ray.origin.z * ray.direction.z / (ellipse->c * ellipse->c);
	c = ray.origin.x * ray.origin.x / (ellipse->a * ellipse->a) + \
		ray.origin.y * ray.origin.y / (ellipse->b * ellipse->b) + \
		ray.origin.z * ray.origin.z / (ellipse->c * ellipse->c) - 1.0f;
	ray.origin = v_add_v(ray.origin, obj->origin);
	discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
	{
		hit.distance = -1.0f;
		return (hit);
	}
	hit.distance = (-b - sqrt(discriminant)) / (2.0f * a);
	if (hit.distance < 0.0f)
		hit.distance = (-b + sqrt(discriminant)) / (2.0f * a);
	hit.position = v_add_v(ray.origin, v_mul_f(ray.direction, hit.distance));
	hit.normal = normalize(v_sub_v(hit.position, obj->origin));
	return (hit);
}

t_hit_info	hit_plane(t_ray ray, t_objects *obj, t_plane *plane)
{
	t_hit_info	hit;
	float		denom;

	denom = v_dot(plane->normal, ray.direction);
	if (denom == 0)
	{
		hit.distance = -1.0f;
		return (hit);
	}
	hit.distance = v_dot(v_sub_v(obj->origin, ray.origin), \
						plane->normal) / denom;
	hit.position = v_add_v(ray.origin, v_mul_f(ray.direction, hit.distance));
	hit.normal = plane->normal;
	return (hit);
}

t_hit_info	hit_quad(t_ray ray, t_objects *obj, t_quad *quad)
{
	float		denom;
	float		beta;
	float		alpha;
	t_hit_info	hit_info;
	t_vec3f		planar_hitpt_vector;

	denom = v_dot(quad->normal, ray.direction);
	hit_info.distance = -1.0f;
	if (fabs(denom) < 1e-8)
		return (hit_info);
	hit_info.distance = (quad->d - v_dot(ray.origin, quad->normal)) / denom;
	if (hit_info.distance < 0.0f)
		return (hit_info);
	hit_info.position = v_add_v(ray.origin, \
		v_mul_f(ray.direction, hit_info.distance));
	planar_hitpt_vector = v_sub_v(hit_info.position, obj->origin);
	alpha = v_dot(quad->w, \
			v_cross(planar_hitpt_vector, normalize(quad->right_corner)));
	beta = v_dot(quad->w, \
			v_cross(normalize(quad->up_corner), planar_hitpt_vector));
	if (alpha < 0 || beta < 0 || alpha > v_length(quad->up_corner) || \
		beta > v_length(quad->right_corner))
	{
		hit_info.distance = -1.0f;
		return (hit_info);
	}
	hit_info.normal = v_mul_f(quad->normal, -ft_sign(denom));
	return (hit_info);
}

t_hit_info	hit_cylinder(t_ray ray, t_objects *o, t_cylinder *cy)
{
	t_hit_info	h;
	t_vec3f		tv[2];
	float		tmp1_f;
	float		g[3];
	float		t[4];

	tv[0] = v_cross(ray.direction, cy->orientation);
	tv[1] = v_cross(v_sub_v(ray.origin, o->origin), cy->orientation);
	g[0] = v_dot(tv[0], tv[0]);
	g[1] = 2.0 * v_dot(tv[0], tv[1]);
	g[2] = v_dot(tv[1], tv[1]) - (cy->diameter / 2.0) * (cy->diameter / 2.0);
	t[0] = (-g[1] - sqrt(g[1] * g[1] - 4.0 * g[0] * g[2])) / (2.0 * g[0]);
	t[1] = (-g[1] + sqrt(g[1] * g[1] - 4.0 * g[0] * g[2])) / (2.0 * g[0]);
	ft_swap(&t[0], &t[1], t[0] > t[1]);
	tmp1_f = v_dot(v_sub_v(v_add_v(o->origin, v_mul_f(cy->orientation,
				(cy->height / 2.0))), ray.origin), cy->orientation);
	t[2] = (tmp1_f + (cy->height / 2)) / v_dot(ray.direction, cy->orientation);
	t[3] = (tmp1_f - (cy->height / 2)) / v_dot(ray.direction, cy->orientation);
	ft_swap(&t[2], &t[3], t[2] > t[3]);
	h.distance = fmin(fmax(t[0], t[2]), fmin(t[1], t[3]));
	if (t[2] > t[1] || t[3] < t[0])
		h.distance = -1.0f;
	if (h.distance <= 0)
		return (h);
	h.position = v_add_v(ray.origin, v_mul_f(ray.direction, h.distance));
	if (t[2] < t[0])
		h.normal = normalize(v_sub_v(v_sub_v(h.position, o->origin), v_mul_f(\
	cy->orientation, v_dot(v_sub_v(h.position, o->origin), cy->orientation))));
	else
		h.normal = v_mul_f(cy->orientation, -ft_sign(v_dot(ray.direction, cy->orientation)));
	return (h);
}

int	triangle_test(t_hit_info hit_info, t_triangle *tri, t_objects *obj, int abc)
{
	t_vec3f		diff;
	t_vec3f		vert;
	float		test;

	if (abc == 0)
		vert = obj->origin;
	else if (abc == 1)
		vert = tri->pb;
	else
		vert = tri->pc;
	diff = v_sub_v(hit_info.position, vert);
	test = v_dot(tri->normal, v_cross(tri->edge[abc], diff));
	return (test >= 0);
}

t_hit_info	hit_triangle(t_ray ray, t_objects *obj, t_triangle *tri)
{
	t_hit_info	h;
	float		pl;

	h.distance = -1.0f;
	pl = v_dot(tri->normal, ray.direction);
	if (fabs(pl) < 0)
		return (h);
	h.distance = -(v_dot(tri->normal, ray.origin) + tri->traverse) / pl;
	if (h.distance < 0)
		return (h);
	h.position = v_add_v(ray.origin, v_mul_f(ray.direction, h.distance));
	if (!triangle_test(h, tri, obj, 0)
		|| !triangle_test(h, tri, obj, 1)
		|| !triangle_test(h, tri, obj, 2))
	{
		h.distance = -1.0f;
		return (h);
	}
	h.normal = tri->normal;
	if (pl > 0)
		h.normal = v_mul_f(h.normal, -1);
	return (h);
}

t_hit_info	hit_objects(t_ray ray, t_objects *obj)
{
	t_hit_info	hit_info;

	if (obj->type == OBJ_SPHER)
		return (hit_sphere(ray, obj, obj->sphere));
	else if (obj->type == OBJ_PLANE)
		return (hit_plane(ray, obj, obj->plane));
	else if (obj->type == OBJ_CYLIN)
		return (hit_cylinder(ray, obj, obj->cylinder));
	else if (obj->type == OBJ_QUADS)
		return (hit_quad(ray, obj, obj->quad));
	else if (obj->type == OBJ_ELLIP)
		return (hit_ellipse(ray, obj, obj->ellipse));
	else if (obj->type == OBJ_PORTA)
		return (hit_quad(ray, obj, &obj->portal->quad));
	else if (obj->type == OBJ_TRIAN)
		return (hit_triangle(ray, obj, obj->triangle));
	hit_info.distance = -1.0f;
	return (hit_info);
}
