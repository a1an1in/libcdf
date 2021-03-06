#ifndef __TEST_H__
#define __TEST_H__

#include <libargs/cmd_args.h>

extern void test_share_mem_write();
extern void test_share_mem_read();
extern int test_unix_tcp_server();
extern int test_unix_tcp_client_send();
extern int test_unix_udp_client_send();
extern int test_unix_udp_client_recieve();
extern void test_state_machine();
extern void test_map();
extern void test_datastructure_hashlist();
extern int test_datastructure_link_list();
extern int test_datastructure_rbtree_map(void);
extern int test_datastructure_vector();
extern void test_datastructure_ring_buffer();
extern int test_inet_server();
extern int test_inet_tcp_client_send();
extern int test_inet_udp_client_recieve();
extern int test_inet_udp_client_send();
extern int test_inet_udp_client_broadcast();
extern void test_ctr_alloc();
extern int test_tmr_user();
extern void test_io_user();
extern int test_io();
extern int test_signal();
extern int test_time();
extern int test_pdt_proto_analyzer();
extern void test_client_recv_of_inet_udp();
extern void test_client_recv_of_unix_udp();
extern int test_server_of_unix();
extern int test_server_of_inet();
extern void test_pa_admin();
extern void test_blob();
extern void test_bus_daemon();
extern void test_bus_client();
extern void test_bus_server();
extern void bus_debug_service();
extern void bus_debug_client(char *bussiness, char *sw, char *level);
extern int test_array_stack();
extern int test_miscellany_net();
extern int lab();
extern int lab2();
extern int lab6(int argc,char **argv);
extern void args_print_help_test_info(args_processor_t *p);
extern int test_json(void);
extern int test_inet_wireless(int argc, char **argv);
extern int test_configurator(void);  

#endif
