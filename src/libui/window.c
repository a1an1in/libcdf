/**
 * @file window.c
 * @synopsis 
 * @author a1an1in@sina.com
 * @version 
 * @date 2016-12-03
 */
#include <libui/window.h>

static int __construct(Window *window,char *init_str)
{
	dbg_str(DBG_SUC,"window construct, window addr:%p",window);
    window->create_graph(window, NULL);
    window->create_font(window,NULL);
    window->create_event(window);
    window->create_background(window, NULL);

    window->open_window(window);

	return 0;
}

static int __deconstrcut(Window *window)
{
	dbg_str(DBG_SUC,"window deconstruct,window addr:%p",window);
    window->close_window(window);
    window->destroy_background(window);
    window->destroy_event(window);
    window->destroy_font(window);
    window->destroy_graph(window);

	return 0;
}

static int __set(Window *window, char *attrib, void *value)
{
	if(strcmp(attrib, "set") == 0) {
		window->set = value;
    } else if(strcmp(attrib, "get") == 0) {
		window->get = value;
	} else if(strcmp(attrib, "construct") == 0) {
		window->construct = value;
	} else if(strcmp(attrib, "deconstruct") == 0) {
		window->deconstruct = value;
	} else if(strcmp(attrib, "move") == 0) {
		window->move = value;
	} else if(strcmp(attrib, "create_font") == 0) {
		window->create_font = value;
	} else if(strcmp(attrib, "destroy_font") == 0) {
		window->destroy_font = value;
	} else if(strcmp(attrib, "create_graph") == 0) {
		window->create_graph = value;
	} else if(strcmp(attrib, "destroy_graph") == 0) {
		window->destroy_graph = value;
	} else if(strcmp(attrib, "create_event") == 0) {
		window->create_event = value;
	} else if(strcmp(attrib, "destroy_event") == 0) {
		window->destroy_event = value;
	} else if(strcmp(attrib, "create_background") == 0) {
		window->create_background = value;
	} else if(strcmp(attrib, "destroy_background") == 0) {
		window->destroy_background = value;
	} else if(strcmp(attrib, "open_window") == 0) {
		window->open_window = value;
	} else if(strcmp(attrib, "close_window") == 0) {
		window->close_window = value;
	} else if(strcmp(attrib, "name") == 0) {
        strncpy(window->name,value,strlen(value));
	} else if(strcmp(attrib, "graph_type") == 0) {
        window->graph_type = *((uint8_t *)value);
	} else if(strcmp(attrib, "screen_width") == 0) {
        window->screen_width = *((uint32_t *)value);
	} else if(strcmp(attrib, "screen_height") == 0) {
        window->screen_height = *((uint32_t *)value);
	} else if(strcmp(attrib, "graph") == 0) {
		window->graph = value;
	} else {
		dbg_str(DBG_DETAIL,"window set, not support %s setting",attrib);
	}

	return 0;
}

static void *__get(Window *obj, char *attrib)
{
    if(strcmp(attrib, "name") == 0) {
        return obj->name;
	} else if(strcmp(attrib, "graph_type") == 0) {
        return &obj->graph_type;
	} else if(strcmp(attrib, "graph") == 0) {
		return obj->graph;
	} else if(strcmp(attrib, "screen_width") == 0) {
		return &obj->screen_width;
	} else if(strcmp(attrib, "screen_height") == 0) {
		return &obj->screen_height;
    } else {
        dbg_str(DBG_WARNNING,"window get, \"%s\" getting attrib is not supported",attrib);
        return NULL;
    }
    return NULL;
}


static class_info_entry_t window_class_info[] = {
	[0 ] = {ENTRY_TYPE_OBJ,"Component","component",NULL,sizeof(void *)},
	[1 ] = {ENTRY_TYPE_FUNC_POINTER,"","set",__set,sizeof(void *)},
	[2 ] = {ENTRY_TYPE_FUNC_POINTER,"","get",__get,sizeof(void *)},
	[3 ] = {ENTRY_TYPE_FUNC_POINTER,"","construct",__construct,sizeof(void *)},
	[4 ] = {ENTRY_TYPE_FUNC_POINTER,"","deconstruct",__deconstrcut,sizeof(void *)},
	[5 ] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","move",NULL,sizeof(void *)},
	[6 ] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","create_font",NULL,sizeof(void *)},
	[7 ] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","destroy_font",NULL,sizeof(void *)},
	[8 ] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","create_graph",NULL,sizeof(void *)},
	[9 ] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","destroy_graph",NULL,sizeof(void *)},
	[10] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","create_event",NULL,sizeof(void *)},
	[11] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","destroy_event",NULL,sizeof(void *)},
	[12] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","create_background",NULL,sizeof(void *)},
	[13] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","destroy_background",NULL,sizeof(void *)},
	[14] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","open_window",NULL,sizeof(void *)},
	[15] = {ENTRY_TYPE_VIRTUAL_FUNC_POINTER,"","close_window",NULL,sizeof(void *)},
	[16] = {ENTRY_TYPE_STRING,"char","name",NULL,0},
	[17] = {ENTRY_TYPE_UINT8_T,"uint8_t","graph_type",NULL,0},
	[18] = {ENTRY_TYPE_UINT32_T,"","screen_width",NULL,sizeof(short)},
	[19] = {ENTRY_TYPE_UINT32_T,"","screen_height",NULL,sizeof(short)},
	[20] = {ENTRY_TYPE_NORMAL_POINTER,"Graph","graph",NULL,sizeof(float)},
	[21] = {ENTRY_TYPE_END},
};
REGISTER_CLASS("Window",window_class_info);

