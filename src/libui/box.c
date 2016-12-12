/**
 * @file box.c
 * @synopsis 
 * @author alan(a1an1in@sina.com)
 * @version 1
 * @date 2016-11-21
 */
#include <libui/box.h>
#include <libui/sdl_window.h>
#include <libui/character.h>

static int __construct(Box *box,char *init_str)
{
	dbg_str(DBG_SUC,"box construct, box addr:%p",box);

	return 0;
}

static int __deconstrcut(Box *box)
{
	dbg_str(DBG_SUC,"box deconstruct,box addr:%p",box);

	return 0;
}

static int __set(Box *box, char *attrib, void *value)
{
	if(strcmp(attrib, "set") == 0) {
		box->set = value;
    } else if(strcmp(attrib, "get") == 0) {
		box->get = value;
	} else if(strcmp(attrib, "construct") == 0) {
		box->construct = value;
	} else if(strcmp(attrib, "deconstruct") == 0) {
		box->deconstruct = value;
	} else if(strcmp(attrib, "draw") == 0) {
		box->draw = value;
	} else if(strcmp(attrib, "load_resources") == 0) {
		box->load_resources = value;
	} else if(strcmp(attrib, "text_input") == 0) {
		box->text_input = value;
	} else if(strcmp(attrib, "name") == 0) {
        strncpy(box->name,value,strlen(value));
	} else {
		dbg_str(DBG_DETAIL,"box set, not support %s setting",attrib);
	}

	return 0;
}

static void *__get(Box *obj, char *attrib)
{
    if(strcmp(attrib, "name") == 0) {
        return obj->name;
    } else {
        dbg_str(DBG_WARNNING,"box get, \"%s\" getting attrib is not supported",attrib);
        return NULL;
    }
    return NULL;
}

static int __draw(Component *component, void *graph)
{
	Graph *g = (Graph *)graph;
	Subject *s = (Subject *)component;
	Box *l = (Box *)component;

	dbg_str(DBG_SUC,"%s draw", ((Obj *)component)->name);

	g->render_set_color(g,0x0,0x0,0x0,0xff);
	g->render_draw_rect(g,s->x,s->y,s->width,s->height);
	/*
	 *l->text = g->render_load_text(g,component->name,g->font, 0,0,0,0xff);
	 *g->render_write_text(g,s->x + 1,s->y + 1,l->text);
	 */
}

static int __text_input(Component *component,char c, void *graph)
{
	Graph *g = (Graph *)graph;
	Character *character;
	dbg_str(DBG_SUC,"text input");
	static int pos_x, pos_y;

	character = g->render_load_character(g,c,g->font, 0,0,0,0xff);
	g->render_write_character(g,pos_x,33,character);
	pos_x += character->width;
	g->render_present(g);

}

static int __load_resources(Component *component,void *graph)
{
}

static class_info_entry_t box_class_info[] = {
	[0] = {ENTRY_TYPE_OBJ,"Component","component",NULL,sizeof(void *)},
	[1] = {ENTRY_TYPE_FUNC_POINTER,"","set",__set,sizeof(void *)},
	[2] = {ENTRY_TYPE_FUNC_POINTER,"","get",__get,sizeof(void *)},
	[3] = {ENTRY_TYPE_FUNC_POINTER,"","construct",__construct,sizeof(void *)},
	[4] = {ENTRY_TYPE_FUNC_POINTER,"","deconstruct",__deconstrcut,sizeof(void *)},
	[5] = {ENTRY_TYPE_FUNC_POINTER,"","draw",__draw,sizeof(void *)},
	[6] = {ENTRY_TYPE_FUNC_POINTER,"","load_resources",__load_resources,sizeof(void *)},
	[7] = {ENTRY_TYPE_FUNC_POINTER,"","text_input",__text_input,sizeof(void *)},
	[8] = {ENTRY_TYPE_STRING,"char","name",NULL,0},
	[9] = {ENTRY_TYPE_END},

};
REGISTER_CLASS("Box",box_class_info);

char *gen_box_setting_str()
{
    cjson_t *root,*b, *c, *e, *s;
    char *set_str;

    root = cjson_create_object();{
        cjson_add_item_to_object(root, "Box", b = cjson_create_object());{
            cjson_add_item_to_object(root, "Component", c = cjson_create_object());{
                cjson_add_item_to_object(root, "Container", e = cjson_create_object());{
                    cjson_add_item_to_object(e, "Subject", s = cjson_create_object());{
                        cjson_add_number_to_object(s, "x", 0);
                        cjson_add_number_to_object(s, "y", 0);
                        cjson_add_number_to_object(s, "width", 500);
                        cjson_add_number_to_object(s, "height", 500);
                    }
                }
				cjson_add_string_to_object(c, "name", "box");
            }
        }
    }
    set_str = cjson_print(root);

    return set_str;
}

void test_ui_box()
{
    Window *window;
    Container *container;
	Graph *g;
    Subject *subject;
	Event *event;
	allocator_t *allocator = allocator_get_default_alloc();
    char *set_str;
    char buf[2048];

    set_str = gen_window_setting_str();
    window  = OBJECT_NEW(allocator, Sdl_Window,set_str);
	g       = window->graph;
    event   = window->event;
    container = (Container *)window;
    object_dump(window, "Sdl_Window", buf, 2048);
    dbg_str(DBG_DETAIL,"Window dump: %s",buf);

    set_str = gen_box_setting_str();
    subject = OBJECT_NEW(allocator, Box,set_str);
    object_dump(subject, "Box", buf, 2048);
    dbg_str(DBG_DETAIL,"Box dump: %s",buf);

    container->add_component(container,subject);
	/*
     *container->search_component(container,"box");
	 */
	dbg_str(DBG_DETAIL,"window container :%p",container);

	window->update_window(window);

    event->poll_event(event, window);

    object_destroy(window);

    free(set_str);

}


