/**
 * @file test_client_send.c
 * @synopsis 
 * @author a1an1in@sina.com
 * @version 1.0
 * @date 2016-09-04
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
#include <libconcurrent/concurrent.h>
#include <libipc/net/inet_client.h>
#include <miscellany/net.h>


static int process_task_callback(client_task_t *task)
{
    dbg_str(DBG_DETAIL,"process_task begin,client send recv data");
    dbg_buf(DBG_DETAIL,"task buffer:",task->buffer,task->buffer_len);
    dbg_str(DBG_DETAIL,"process_task end");
}

int test_inet_udp_client_send()
{
    client_t *cli;
    char buf[] = {1,2,3,4,5,6,7,8,9,10};
    allocator_t *allocator = allocator_get_default_alloc();

    cli = inet_udp_client(allocator,
                          "127.0.0.1",//char *host,
                          "2016",//char *client_port,
                          process_task_callback,
                          NULL);

    inet_udp_client_send(cli,//client_t *client,
                         buf,//const void *buf,
                         sizeof(buf),
                         0,//int flags,
                         "127.0.0.1",//char *host,
                         "1989");//char *client_port,

    inet_client_destroy(cli);
}

/**
 *test broadcast, the client addr must be 0.0.0.0, 
 *if not it cant recv data
 */
int test_inet_udp_client_broadcast()
{
    client_t *cli;
    char buf[] = {1,2,3,4,5,6,7,8,9,10};
    char ipaddr[50];
    struct sockaddr_in raddr;
    socklen_t destlen;
    allocator_t *allocator = allocator_get_default_alloc();

    cli = inet_udp_client(allocator,
                          "192.168.20.87",//char *host,
                          "1989",//char *client_port,
                          process_task_callback,
                          NULL);

    get_broadcast_addr("eth0", ipaddr);
    dbg_str(DBG_DETAIL,"boradcast addr:%s",ipaddr);
    inet_udp_client_broadcast(cli,ipaddr, (char *)"1989",buf,sizeof(buf));
}
int test_inet_tcp_client_send()
{
    client_t *cli;
    const char buf[] = {1,2,3,4,5,6,7,8,9,10};
    struct sockaddr_in raddr;
    socklen_t destlen;
    allocator_t *allocator = allocator_get_default_alloc();

    cli = inet_tcp_client(allocator,
                          "127.0.0.1",//char *host,
                          "6888",//char *client_port,
                          process_task_callback,
                          NULL);

    raddr.sin_family = AF_INET; 
    raddr.sin_port = htons(atoi("6888"));  
    inet_pton(AF_INET,"0.0.0.0",&raddr.sin_addr);

    inet_tcp_client_send(cli,//client_t *client,
                         buf,//const void *buf,
                         sizeof(buf),0);//socklen_t destlen);

    sleep(10);

    inet_client_destroy(cli);

}
