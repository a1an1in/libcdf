#include <libipc/net/client.h>
#include <libbus/bus.h>

void test_bus_client()
{
    allocator_t *allocator = allocator_get_default_alloc();
    bus_t *bus;
#if 0
    char *server_host = "bus_server_path";
    char *server_srv = NULL;
#else
    char *server_host = "192.168.20.122";
    char *server_srv = "12345";
#endif
	char buf[1024] = "hello world!";
	int buf_len = strlen(buf);
	char out[1024];
    uint8_t out_len;
    /*
     *char *args[2] = {"abc","hello world!"};
     */
    bus_method_args_t args[2] = {
        [0] = {ARG_TYPE_INT32,"id", "123"},
        [1] = {ARG_TYPE_STRING,"content", "hello_world"},
    };
    
    dbg_str(DBG_DETAIL,"test_bus_client");

    bus = bus_client_create(allocator,
                            server_host,
                            server_srv,
                            CLIENT_TYPE_TCP_INET);
    /*
     *bus_lookup(bus, "test");
     */

    /*
     *bus_invoke(bus,"test", "hello",2, args);
     */

    bus_invoke_sync(bus,"test", "hello",2, args,out,&out_len);
	
}