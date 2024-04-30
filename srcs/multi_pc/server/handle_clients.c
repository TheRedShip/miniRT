/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomoron <tomoron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 02:12:14 by tomoron           #+#    #+#             */
/*   Updated: 2024/04/30 20:31:26 by tomoron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*buffer_to_str(t_buffer *buffer)
{
	char	*res;
	int		len;

	len = get_buffer_str_len(buffer);
	if(len != (unsigned)(WIDTH * HEIGHT * sizeof(t_vec3f)))
	{
		free_buffer(buffer);
		fprintf(stderr, "invalid data size\nexpected :%d\ngot :%d", \
		(int)(WIDTH * HEIGHT * sizeof(t_vec3f)), len);
		return(0);
	}
	res = malloc(len);
	if(!res)
		fprintf(stderr, "malloc failed\n");
	if (res)
		copy_buffer(res, buffer);
	free_buffer(buffer);
	return(res);
}

char	*get_client_data(int fd)
{
	t_buffer	*buffer;
	int			read_len;

	buffer = 0;
	read_len = SOCKET_BUFFER_SIZE - 1;
	while (read_len)
	{
		if (!add_buffer(&buffer))
		{
			fprintf(stderr, "too much data from fd %d\n", fd);
			free_buffer(buffer);
			return (0);
		}
		read_len = read(fd, buffer->str, SOCKET_BUFFER_SIZE);
		buffer->len = read_len;
	}
	return (buffer_to_str(buffer));
}


void	add_to_acc_img(t_vec3f *data, t_scene *scene)
{
	int i;
	int j;

	i = 0;
	while(i < HEIGHT)
	{
		j = 0;
		while(j < WIDTH)
		{
			scene->mlx->acc_img[i][j] = vec3f_add_v(scene->mlx->acc_img[i][j],\
			data[i * WIDTH + j]);
			scene->mlx->final_img[i][j] = vec3f_div_f(scene->mlx->acc_img[i][j], scene->mlx->frame_index);
			j++;
		}
		i++;
	}
}

void	render_new_acc(t_scene *scene)
{
	rt_render_image(bloom(scene, scene->mlx->final_img), &scene->mlx->img);
	mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->win, scene->mlx->img.img, 0, 0);
}

void	*handle_client(void *data)
{
	int			client_fd;
	t_scene		*scene;
	char		*client_data;
	t_thread_data	*patate;

	patate = data;
	client_fd = patate->fd;
	scene = patate->scene;
	printf("connection :%d\n", client_fd);
	client_data = get_client_data(client_fd);
	close(client_fd);
	pthread_mutex_lock(&scene->server.img_mutex);
	scene->mlx->frame_index++;
	if(client_data)
		add_to_acc_img((t_vec3f *)client_data,scene); 
	free(client_data);
	render_new_acc(scene);
	pthread_mutex_unlock(&scene->server.img_mutex);
	return (0);
}

void	wait_clients(t_scene *scene, int socket)
{
	struct sockaddr_in	client;
	socklen_t			client_addr_len;
	long int			client_fd;
	t_srv_threads		*threads;
	t_thread_data		*thread_data;

	printf("size: %lu\n", sizeof(t_vec3f));
	client_addr_len = sizeof(struct sockaddr_in);
	threads = 0;
	while (1)
	{
		client_fd = accept(socket, (struct sockaddr *)&client, \
		&client_addr_len);
		if (client_fd < 0)
			perror("accept");
		else
		{
			thread_data = malloc(sizeof(t_thread_data));
			if(!thread_data)
			{
				close(client_fd);
				continue ; 
			}
			thread_data->fd = client_fd;
			thread_data->scene = scene;
			if (!add_thread(&threads) || pthread_create(&threads->id, NULL, \
			handle_client, (void *)thread_data))
			{
				close(client_fd);
				continue ;
			}
		}
	}
	wait_threads(threads);
}