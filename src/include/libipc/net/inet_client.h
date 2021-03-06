#ifndef __INET_CLIENT_H__
#define __INET_CLIENT_H__

#include <libconcurrent/concurrent.h>
#include <libconcurrent/io_user.h>

/*
 *enum socktype_e{
 *    SOCKTYPE_UDP = 0,
 *    SOCKTYPE_TCP,
 *    SOCKTYPE_UNIX
 *};
 */

/*
 *struct user_s;
 */
typedef struct io_user_s client_t;

typedef struct client_task_s{
	uint32_t fd;
	char key[10];
	struct event *event;
	allocator_t *allocator;
	concurrent_slave_t *slave;
	uint8_t *buffer;
	uint32_t buffer_len;
	client_t *client;
}client_task_t;


client_t *inet_udp_client(allocator_t *allocator,
					      char *host,
					      char *client_port,
					      int (*process_task_cb)(client_task_t *task),
					      void *opaque);

client_t *inet_tcp_client(allocator_t *allocator,
					      char *server_ip,
					      char *server_port,
					      int (*process_task_cb)(client_task_t *task),
					      void *opaque);

int inet_client_destroy(client_t *client);

int inet_udp_client_send(client_t *client,
                         void *buf,
                         size_t nbytes,
                         int flags,
                         char *dest_id_str, 
                         char *dest_srv_str);

int inet_udp_client_broadcast(client_t *cli, char *broadcast_addr, char *dest_port,void *buf,uint32_t len);

int inet_tcp_client_send(client_t *client,const void *buf,size_t nbytes,int flags);
#endif
