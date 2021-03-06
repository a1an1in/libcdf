/**
 * @file client.c
 * @synopsis 
 * @author a1an1in@sina.com
 * @version 
 * @date 2016-10-09
 */
/* Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>     /* basic system data types */
#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>     /* inet(3) functions */
#include <sys/epoll.h>     /* epoll function */
#include <fcntl.h>         /* nonblocking */
#include <sys/resource.h>  /*setrlimit */
#include <signal.h>
#include <libipc/net/client.h>
#include <libdbg/debug.h>

client_t *client(allocator_t *allocator,
                 char *type,
                 char *id_str,
                 char *serve_no,
                 int (*process_task_cb)(client_task_t *task),
                 void *opaque)
{
    client_t *c;

    //there may add id_str check....

    if(!strcmp(type,CLIENT_TYPE_INET_TCP)){
        c = inet_tcp_client(allocator, id_str, serve_no,
                            process_task_cb, opaque);
    } else if (!strcmp(type,CLIENT_TYPE_INET_UDP)){
        c = inet_udp_client(allocator, id_str, serve_no,
                            process_task_cb, opaque);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_UDP)){
        c = unix_udp_client(allocator, id_str, process_task_cb, opaque);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_TCP)){
        c = unix_tcp_client(allocator, id_str, process_task_cb, opaque);
    } else {
        dbg_str(DBG_WARNNING,"client error type");
        return NULL;
    }

    strncpy(c->type_str,type,strlen(type));

    return c;
}
int client_send(client_t *client,
                const void *buf,
                size_t nbytes,
                int flags,
                char *dest_id_str, 
                char *dest_srv_str)
{
    int ret;
    char *type = client->type_str;

    if(!strcmp(type,CLIENT_TYPE_INET_TCP)){
        ret = inet_tcp_client_send(client,buf,nbytes,flags);
    } else if (!strcmp(type,CLIENT_TYPE_INET_UDP)){
        inet_udp_client_send(client,(char *)buf, nbytes, flags, dest_id_str, dest_srv_str);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_UDP)){
        ret = unix_udp_client_send(client,buf,nbytes,flags, dest_id_str);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_TCP)){
        ret = unix_tcp_client_send(client,buf,nbytes,flags);
    } else {
        dbg_str(DBG_WARNNING,"client error type");
        return -1;
    }

    return ret;
}
int client_destroy(client_t *client)
{
    char *type = client->type_str;

    if(!strcmp(type,CLIENT_TYPE_INET_TCP)){
        inet_client_destroy(client);
    } else if (!strcmp(type,CLIENT_TYPE_INET_UDP)){
        inet_client_destroy(client);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_UDP)){
        unix_client_destroy(client);
    } else if (!strcmp(type,CLIENT_TYPE_UNIX_TCP)){
        unix_client_destroy(client);
    } else {
        dbg_str(DBG_WARNNING,"client error type");
        return -1;
    }

    return 0;
}

static int process_task_callback(client_task_t *task)
{
    dbg_str(DBG_DETAIL,"process_task begin,client recv");
    /*
     *user_t *user = task->client;
     *void *opaque = user->opaque;
     */
    dbg_buf(DBG_VIP,"task buffer:",task->buffer,task->buffer_len);
    dbg_str(DBG_DETAIL,"process_task end");
}

void test_client_recv_of_inet_udp()
{
    allocator_t *allocator = allocator_get_default_alloc();

    dbg_str(DBG_DETAIL,"test_client_recv_of_inet_udp");

    client(allocator,
           CLIENT_TYPE_INET_UDP,
           "127.0.0.1",//char *host,
           "1989",//char *client_port,
           process_task_callback,
           NULL);
}
void test_client_recv_of_unix_udp()
{
    allocator_t *allocator = allocator_get_default_alloc();

    dbg_str(DBG_DETAIL,"test_client_recv_of_inet_udp");

    client(allocator,
           CLIENT_TYPE_UNIX_UDP,
           "test_client_unix_path",//char *host,
           NULL,//char *client_port,
           process_task_callback,
           NULL);
}
