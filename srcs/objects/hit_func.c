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

t_hit_info	hit_sphere(t_ray r, t_objects *obj, t_sphere *sphere)
{
	float		a;
	float		b;
	float		c;
	t_hit_info	hit_info;
	float		discriminant;

	a = v_dot(r.direction, r.direction);
	b = (2 * r.origin.x * r.direction.x - 2 * r.direction.x * obj->origin.x) + \
		(2 * r.origin.y * r.direction.y - 2 * r.direction.y * obj->origin.y) + \
		(2 * r.origin.z * r.direction.z - 2 * r.direction.z * obj->origin.z);
	c = (r.origin.x * r.origin.x - 2 * r.origin.x * obj->origin.x + obj->origin.x * obj->origin.x) + 
		(r.origin.y * r.origin.y - 2 * r.origin.y * obj->origin.y + obj->origin.y * obj->origin.y) + 
		(r.origin.z * r.origin.z - 2 * r.origin.z * obj->origin.z + obj->origin.z * obj->origin.z) - 
		(sphere->diameter / 2 * sphere->diameter / 2);
	discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
	{
		hit_info.distance = -1.0f;
		return (hit_info);
	}
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
	t_hit_info	hit_info;
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
		hit_info.distance = -1.0f;
		return (hit_info);
	}
	hit_info.distance = (-b - sqrt(discriminant)) / (2.0f * a);
	if (hit_info.distance < 0.0f)
		hit_info.distance = (-b + sqrt(discriminant)) / (2.0f * a);
	hit_info.position = v_add_v(ray.origin, v_mul_f(ray.direction, hit_info.distance));
	hit_info.normal = normalize(v_sub_v(hit_info.position, obj->origin));
	return (hit_info);
}

t_hit_info	hit_plane(t_ray ray, t_objects *obj, t_plane *plane)
{
	t_hit_info	hit_info;
	float		denom;

	denom = v_dot(plane->normal, ray.direction);
	if (denom == 0)
	{
		hit_info.distance = -1.0f;
		return (hit_info);
	}
	hit_info.distance = v_dot(v_sub_v(obj->origin, ray.origin), \
						plane->normal) / denom;
	hit_info.position = v_add_v(ray.origin, v_mul_f(ray.direction, hit_info.distance));
	hit_info.normal = plane->normal;
	return (hit_info);
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

t_hit_info	hit_cylinder(t_ray ray, t_objects *obj, t_cylinder *cylinder)
{
	t_hit_info	hit_info;

	t_vec3f tmp1 = v_cross(ray.direction, cylinder->orientation);
	t_vec3f	tmp_vect = v_sub_v(ray.origin, obj->origin);
	t_vec3f	tmp2 = v_cross(tmp_vect, cylinder->orientation);
	
	float	a = v_dot(tmp1, tmp1);
	float	b = 2.0 * v_dot(tmp1, tmp2);
	float	c = v_dot(tmp2, tmp2) - (cylinder->diameter / 2.0) * (cylinder->diameter / 2.0);
	float	discriminant = b*b - 4.0 * a * c;

	float	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	float	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t1 > t2)
	{
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	t_vec3f	mid_vect = v_mul_f(cylinder->orientation, (cylinder->height / 2.0));
	t_vec3f	mid_point = v_add_v(obj->origin, mid_vect);
	float	tmp1_bis = v_dot(v_sub_v(mid_point, ray.origin), cylinder->orientation);
	float	tmp2_bis = v_dot(ray.direction, cylinder->orientation);
	float	t3 = (tmp1_bis + (cylinder->height / 2.0)) / tmp2_bis;
	float	t4 = (tmp1_bis - (cylinder->height / 2.0)) / tmp2_bis;
	if (t3 > t4)
	{
		float tmp = t3;
		t3 = t4;
		t4 = tmp;
	}
	if (t3 > t2 || t4 < t1)
	{
		hit_info.distance = -1.0f;
		return (hit_info);
	}
	float	t_final = fmax(t1, t3);
	if (t_final < 0)
		t_final = fmin(t2, t4);
	if (t_final <= 0)
	{
		hit_info.distance = -1.0f;
		return (hit_info);
	}
	hit_info.distance = t_final;
	hit_info.position = v_add_v(ray.origin, v_mul_f(ray.direction, hit_info.distance));
	if (t3 < t1)
		hit_info.normal = normalize(v_sub_v(v_sub_v(hit_info.position, \
						obj->origin), v_mul_f(cylinder->orientation, \
						v_dot(v_sub_v(hit_info.position, obj->origin), \
						cylinder->orientation))));
	else
		hit_info.normal = v_mul_f(cylinder->orientation, \
					-ft_sign(v_dot(ray.direction, cylinder->orientation)));
	return (hit_info);
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
	if (fabs(pl) < 1e-6)
		return (h);
	h.distance = -(v_dot(tri->normal, ray.origin) + tri->traverse) / pl;
	if (h.distance < 1e-6)
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