/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/19/2015 5:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
/*  
 * Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 *  
 *  
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
#include <stdio.h>
#include <unistd.h>
#include <libdbg/debug.h>
#include <libargs/cmd_args.h>
#include <constructor_priority.h>
#include <test.h>

#define LIBRARY_VERSION "libcutils version: 2.3.4.0"

#ifndef MAKELIB

typedef struct config_list_s{
}config_list_t;

typedef struct base_s{
	config_list_t config;
	args_processor_t *p;
}base_t;


static int args_process_help(void *base,int argc,char **argv)
{
	args_print_help_info(args_get_processor_globle_addr());
    exit(1);
	return 0;
}

static int args_process_ipaddr(void *base,int argc,char **argv)
{
	dbg_str(DBG_DETAIL,"args_process_ipaddr:%s",argv[0]);
	return 1;
}

static int args_process_port(void *base,int argc,char **argv)
{
	dbg_str(DBG_DETAIL,"args_process_port:%s",argv[0]);
	return 1;
}

static int args_process_test_share_mem_write(void *base,int argc,char **argv)
{
    test_share_mem_write();
	return 0;
}

static int args_process_test_share_mem_read(void *base,int argc,char **argv)
{
    test_share_mem_read();
	return 0;
}

static int args_process_test_unix_tcp_server(void *base,int argc,char **argv)
{
    test_unix_tcp_server();
	return 0;
}

static int args_process_test_unix_tcp_client_send(void *base,int argc,char **argv)
{
    test_unix_tcp_client_send();
	return 0;
}

static int args_process_test_unix_udp_client_send(void *base,int argc,char **argv)
{
    test_unix_udp_client_send();
	return 0;
}

static int args_process_test_unix_udp_client_recieve(void *base,int argc,char **argv)
{
    test_unix_udp_client_recieve();
	return 0;
}

static int args_process_test_state_machine(void *base,int argc,char **argv)
{
    test_state_machine();
	return 0;
}

static int args_process_test_map(void *base,int argc,char **argv)
{
    test_map();
	return 0;
}

static int args_process_test_hashlist(void *base,int argc,char **argv)
{
    test_datastructure_hashlist();
	return 0;
}

static int args_process_test_llist(void *base,int argc,char **argv)
{
    test_datastructure_link_list();
	return 0;
}

static int args_process_test_rbtree_map(void *base,int argc,char **argv)
{
    test_datastructure_rbtree_map();
	return 0;
}

static int args_process_test_vector(void *base,int argc,char **argv)
{
    test_datastructure_vector();
	return 0;
}

static int args_process_test_ring_buffer(void *base,int argc,char **argv)
{
    test_datastructure_ring_buffer();
	return 0;
}

static int args_process_test_inet_tcp_server(void *base,int argc,char **argv)
{
    test_inet_server();
    return 0;
}

static int args_process_test_inet_tcp_client_send(void *base,int argc,char **argv)
{
    test_inet_tcp_client_send();
    return 0;
}

static int args_process_test_inet_udp_client_recieve(void *base,int argc,char **argv)
{
    test_inet_udp_client_recieve();
    return 0;
}

static int args_process_test_inet_udp_client_send(void *base,int argc,char **argv)
{
    test_inet_udp_client_send();
    return 0;
}

static int args_process_test_allocator(void *base,int argc,char **argv)
{
    test_ctr_alloc();
    return 0;
}

static int args_process_test_tmr_user(void *base,int argc,char **argv)
{
    test_tmr_user();
    return 0;
}

static int args_process_test_io_user(void *base,int argc,char **argv)
{
    test_io_user();
    return 0;
}

static int args_process_test_evio(void *base,int argc,char **argv)
{
     test_io();
    return 0;
}

static int args_process_test_evsignal(void *base,int argc,char **argv)
{
    test_signal();
    /*
     *test_signal2();
     */
    return 0;
}

static int args_process_test_evtime(void *base,int argc,char **argv)
{
    test_time();
	/*
	 *test_time2();
	 */
    return 0;
}

static int args_process_test_analyzer(void *base,int argc,char **argv)
{
    test_pdt_proto_analyzer();
	/*
	 *test_pdu_proto_analyzer();
	 */
    return 0;
}

static int args_process_test_gclient_recv_of_inet_udp(void *base,int argc,char **argv)
{
    test_client_recv_of_inet_udp();
    return 0;
}

static int args_process_test_gclient_recv_of_unix_udp(void *base,int argc,char **argv)
{
    test_client_recv_of_unix_udp();
    return 0;
}

static int args_process_test_gserver_of_unix(void *base,int argc,char **argv)
{
    test_server_of_unix();
    return 0;
}

static int args_process_test_gserver_of_inet(void *base,int argc,char **argv)
{
    test_server_of_inet();
    return 0;
}

static int args_process_lab(void *base,int argc,char **argv)
{
    dbg_str(DBG_DETAIL,"test begin");
    /*
     *lab();
     */
	/*
     *lab2();
	 */
    /*
     *lab3();
     */
    /*
     *lab4();
     */
     lab5();
    dbg_str(DBG_DETAIL,"test end");
    return 0;
}

static int args_process_help_test(void *base,int argc,char **argv)
{
	args_print_help_test_info(args_get_processor_globle_addr());
    exit(1);
	return 0;
}

static int args_process_test_pa_admin(void *base,int argc,char **argv)
{
    test_pa_admin();
	return 0;
}

static int args_process_test_blob(void *base,int argc,char **argv)
{
    test_blob();
	return 0;
}

static int args_process_busd(void *base,int argc,char **argv)
{
    test_bus_daemon();
	return 0;
}

static int args_process_busc(void *base,int argc,char **argv)
{
    test_bus_client();
	return 0;
}

static int args_process_buss(void *base,int argc,char **argv)
{
    test_bus_server();
	return 0;
}

static int args_process_test_as(void *base,int argc,char **argv)
{
    test_array_stack();
	return 0;
}

static int args_process_test_miscellany_net(void *base,int argc,char **argv)
{
    test_miscellany_net();

	return 0;
}

static int args_process_test_obj(void *base,int argc,char **argv)
{
	test_obj();
	return 0;
}

static int args_process_test_obj_subject(void *base,int argc,char **argv)
{
	test_obj_subject();
	return 0;
}

static int args_process_test_obj_enemy(void *base,int argc,char **argv)
{
	test_obj_enemy();
	return 0;
}

static int args_process_test_json(void *base,int argc,char **argv)
{
    test_json();
	return 0;
}

static int args_process_test_sdl(void *base,int argc,char **argv)
{
	/*
     *test_sdl(argc, argv);
	 */
	return 0;
}

static int args_process_test_ui_container(void *base,int argc,char **argv)
{
    test_ui_container();
	return 0;
}

static int args_process_test_ui_button(void *base,int argc,char **argv)
{
    test_ui_button();
	return 0;
}

static int args_process_test_string(void *base,int argc,char **argv)
{
    test_obj_string();
	return 0;
}

static int args_process_test_Map(void *base,int argc,char **argv)
{
    test_obj_map();
	return 0;
}

static int args_process_test_Iterator(void *base,int argc,char **argv)
{
    test_obj_iter();
	return 0;
}

static int args_process_test_Hmap_Iterator(void *base,int argc,char **argv)
{
    test_obj_hiter();
	return 0;
}

static int args_process_test_Hmap(void *base,int argc,char **argv)
{
    test_obj_hash_map();
	return 0;
}

static int args_process_test_Graph(void *base,int argc,char **argv)
{
    test_ui_graph();
	return 0;
}
static int args_process_test_SDL_Window(void *base,int argc,char **argv)
{
	test_ui_sdl_window();
	return 0;
}

static int args_process_test_Image(void *base,int argc,char **argv)
{
	test_obj_image();
	return 0;
}

static int args_process_test_SDL_Image(void *base,int argc,char **argv)
{
	test_obj_sdl_image();
	return 0;
}

static int args_process_test_SDL_Font(void *base,int argc,char **argv)
{
	test_obj_sdl_font();
	return 0;
}

static int args_process_test_SDL_Text(void *base,int argc,char **argv)
{
	/*
	 *test_obj_sdl_text();
	 */
	return 0;
}

static int args_process_test_SDL_Character(void *base,int argc,char **argv)
{
	
	test_obj_sdl_character();
	return 0;
}

static int args_process_test_SDL_Event(void *base,int argc,char **argv)
{
	test_obj_sdl_event();
	return 0;
}

static int args_process_test_Label(void *base,int argc,char **argv)
{
	/*
     *test_ui_label();
	 */
	return 0;
}

static int args_process_test_Text_Area(void *base,int argc,char **argv)
{
	test_ui_text_area();
	return 0;
}

static int args_process_test_LList(void *base,int argc,char **argv)
{
	test_obj_llist_list();
	return 0;
}

static int args_process_test_Text(void *base,int argc,char **argv)
{
	test_obj_text();
	return 0;
}

static int args_process_test_sdl_timer(void *base,int argc,char **argv)
{
	test_obj_sdl_timer();
	return 0;
}
static cmd_config_t cmds[]={
	{"Sdl_Timer", args_process_test_sdl_timer,0, "test", "N/A","test"},
	{"Text", args_process_test_Text,0, "test", "N/A","test"},
	{"LList", args_process_test_LList,0, "test", "N/A","test"},
	{"Text_Area", args_process_test_Text_Area,0, "test", "N/A","test"},
	{"Label", args_process_test_Label,0, "test", "N/A","test"},
	{"SDL_Event", args_process_test_SDL_Event,0, "test", "N/A","test"},
	{"SDL_Char", args_process_test_SDL_Character,0, "test", "N/A","test"},
	{"SDL_Text", args_process_test_SDL_Text,0, "test", "N/A","test"},
	{"SDL_Font", args_process_test_SDL_Font,0, "test", "N/A","test"},
	{"SDL_Image", args_process_test_SDL_Image,0, "test", "N/A","test"},
	{"Image", args_process_test_Image,0, "test", "N/A","test"},
	{"SDL_Window", args_process_test_SDL_Window,0, "test", "N/A","test"},
	{"Graph", args_process_test_Graph,0, "test", "N/A","test"},
	{"HMap", args_process_test_Hmap,0, "test", "N/A","test"},
	{"HIter", args_process_test_Hmap_Iterator,0, "test", "N/A","test"},
	{"Iter", args_process_test_Iterator,0, "test", "N/A","test"},
	{"Map", args_process_test_Map,0, "test", "N/A","test"},
	{"String", args_process_test_string,0, "test", "N/A","test"},
	{"Button", args_process_test_ui_button,0, "test", "N/A","test"},
	{"Container", args_process_test_ui_container,0, "test", "N/A","test"},
	{"sdl", args_process_test_sdl,0, "test", "N/A","test"},
	{"json", args_process_test_json,0, "test", "N/A","test"},
	{"Enemy", args_process_test_obj_enemy,0, "test", "N/A","obj"},
	{"Subject", args_process_test_obj_subject,0, "test", "N/A","obj"},
	{"Obj", args_process_test_obj,0, "test", "N/A","obj"},
	{"miscellany_net", args_process_test_miscellany_net,0, "test", "N/A","miscellany_net"},
	{"as", args_process_test_as,0, "test", "N/A","array_stack"},
	{"buss", args_process_buss,0, "test", "N/A","bus"},
	{"busc", args_process_busc,0, "test", "N/A","bus"},
	{"busd", args_process_busd,0, "app", "N/A","bus"},
	{"blob", args_process_test_blob,0, "test", "N/A","bus"},
	{"pa_admin", args_process_test_pa_admin,0, "test", "N/A","help info"},
	{"help_test", args_process_help_test,0, "help", "N/A","help info"},
	{"lab", args_process_lab,0, "test", "N/A","test simple code"},
	{"general_inet_server", args_process_test_gserver_of_inet,0, "test", "N/A","test general server of inet tcp"},
	{"general_unix_server", args_process_test_gserver_of_unix,0, "test", "N/A","test general server of unix tcp"},
	{"general_unix_client_rcv", args_process_test_gclient_recv_of_unix_udp,0, "test", "N/A","test general client of unix udp"},
	{"general_inet_client_rcv", args_process_test_gclient_recv_of_inet_udp,0, "test", "N/A","test general client of inet udp"},
	{"analyzer", args_process_test_analyzer,0, "test", "N/A","test analyzer"},
	{"evtime", args_process_test_evtime,0, "test", "N/A","test evtime"},
	{"evsignal", args_process_test_evsignal,0, "test", "N/A","test evsignal"},
	{"evio", args_process_test_evio,0, "test", "N/A","test evio"},
	{"io_user", args_process_test_io_user,0, "test", "N/A","test io user"},
	{"tmr_user", args_process_test_tmr_user,0, "test", "N/A","test timer user"},
	{"allocator", args_process_test_allocator,0, "test", "N/A","test_allocator"},
	{"inet_udp_client_send", args_process_test_inet_udp_client_send,0, "test", "N/A","test udp inet client send"},
	{"inet_udp_client_recieve", args_process_test_inet_udp_client_recieve,0, "test", "N/A","test_inet_udp_client_recieve"},
	{"inet_tcp_client_send", args_process_test_inet_tcp_client_send,0, "test", "N/A","test_inet_tcp_client_send"},
	{"inet_tcp_server", args_process_test_inet_tcp_server,0, "test", "N/A","test_inet_tcp_server"},
	{"map", args_process_test_map,0, "test", "N/A","test_map"},
	{"ring_buffer", args_process_test_ring_buffer,0, "test", "N/A","test_ring_buffer"},
	{"vector", args_process_test_vector,0, "test", "N/A","test_vector"},
	{"rbtree_map", args_process_test_rbtree_map,0, "test", "N/A","test_rbtree_map"},
	{"llist", args_process_test_llist,0, "test", "N/A","test_llist"},
	{"hash_map", args_process_test_hashlist,0, "test", "N/A","test_hashlist"},
	{"state_machine", args_process_test_state_machine,0, "test", "N/A","test_state_machine"},
	{"unix_udp_client_recieve", args_process_test_unix_udp_client_recieve,0, "test", "N/A","test_unix_udp_client_recieve"},
	{"unix_udp_client_send", args_process_test_unix_udp_client_send,0, "test", "N/A","test_unix_udp_client_send"},
	{"unix_tcp_client_send", args_process_test_unix_tcp_client_send,0, "test", "N/A","test_unix_tcp_client_send"},
	{"unix_tcp_server", args_process_test_unix_tcp_server,0, "test", "N/A","test_unix_tcp_server"},
	{"shm_read", args_process_test_share_mem_read,0, "test", "N/A","test_share_mem_read"},
	{"shm_write", args_process_test_share_mem_write,0, "test", "N/A","test_share_mem_write"},
	{"port", args_process_port,1, "config", "NN(number)","udp port,using to send/rcv msg with neighbor"},
	{"ip", args_process_ipaddr,1, "config", "xx.xx.xx.xx","ip addr,using to send/rcv msg with neighbor"},
	{"help", args_process_help,0, "help", "N/A","help info"},
	{NULL,NULL,0,NULL,NULL,NULL},
};

/*
 * The libs used modules has been registered before main func,
 * and debugger has been construct before main too. so can use
 * it derectly.
 */
int main(int argc, char *argv[])
{
	int ret = 0;

	dbg_str(DBG_DETAIL,"main func start");

	args_process(NULL,cmds,argc, argv);
	test_ui_text_area();

	dbg_str(DBG_DETAIL,"main func end");
	pause();
	dbg_str(DBG_DETAIL,"main func out,but pause breaked");

	return ret;
}
#endif

__attribute__((constructor(CONSTRUCTOR_PRIORITY_PRINT_LIBRARY_VERSION))) void
print_library_version()
{
    CONSTRUCTOR_PRINT("CONSTRUCTOR_PRIORITY_PRINT_LIBRARY_VERSION=%d,%s\n",
                      CONSTRUCTOR_PRIORITY_PRINT_LIBRARY_VERSION, LIBRARY_VERSION);
}
