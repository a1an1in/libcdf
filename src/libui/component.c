/**
 * @file component.c
 * @synopsis 
 * @author alan(a1an1in@sina.com)
 * @version 1
 * @date 2016-11-21
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
#include <libui/component.h>
#include <libui/window.h>
#include <libui/event.h>

static int __construct(Component *component,char *init_str)
{
    dbg_str(DBG_DETAIL,"component construct, component addr:%p",component);

    return 0;
}

static int __deconstrcut(Component *component)
{
    dbg_str(DBG_DETAIL,"component deconstruct,component addr:%p",component);

    return 0;
}

static int __move(Component *component)
{
    dbg_str(DBG_DETAIL,"component move");
}

static int __set(Component *component, char *attrib, void *value)
{
    if (strcmp(attrib, "set") == 0) {
        component->set = value;
    } else if (strcmp(attrib, "get") == 0) {
        component->get = value;
    } else if (strcmp(attrib, "construct") == 0) {
        component->construct = value;
    } else if (strcmp(attrib, "deconstruct") == 0) {
        component->deconstruct = value;
    }
    else if (strcmp(attrib, "move") == 0) {
        component->move = value;
    } else if (strcmp(attrib, "draw") == 0) {
        component->draw = value;
    } else if (strcmp(attrib, "load_resources") == 0) {
        component->load_resources = value;
    } else if (strcmp(attrib, "text_key_input") == 0) {
        component->text_key_input = value;
    } else if (strcmp(attrib, "backspace_key_input") == 0) {
        component->backspace_key_input = value;
    } else if (strcmp(attrib, "up_key_down") == 0) {
        component->up_key_down = value;
    } else if (strcmp(attrib, "down_key_down") == 0) {
        component->down_key_down = value;
    } else if (strcmp(attrib, "left_key_down") == 0) {
        component->left_key_down = value;
    } else if (strcmp(attrib, "right_key_down") == 0) {
        component->right_key_down = value;
    } else if (strcmp(attrib, "pageup_key_down") == 0) {
        component->pageup_key_down = value;
    } else if (strcmp(attrib, "pagedown_key_down") == 0) {
        component->pagedown_key_down = value;
    } else if (strcmp(attrib, "one_line_up") == 0) {
        component->one_line_up = value;
    } else if (strcmp(attrib, "one_line_down") == 0) {
        component->one_line_down = value;
    } else if (strcmp(attrib, "mouse_button_down") == 0) {
        component->mouse_button_down = value;
    } else if (strcmp(attrib, "mouse_over") == 0) {
        component->mouse_over = value;
    } else if (strcmp(attrib, "mouse_wheel") == 0) {
        component->mouse_wheel = value;
    } else if (strcmp(attrib, "window_moved") == 0) {
        component->window_moved = value;
    } else if (strcmp(attrib, "window_resized") == 0) {
        component->window_resized = value;
    }
    else if (strcmp(attrib, "name") == 0) {
        strncpy(component->name,value,strlen(value));
    } else {
        dbg_str(DBG_DETAIL,"component set, not support %s setting",attrib);
    }

    return 0;
}

static void *__get(Component *obj, char *attrib)
{
    if (strcmp(attrib, "name") == 0) {
        return obj->name;
    } else {
        dbg_str(DBG_WARNNING,"component get, \"%s\" getting attrib is not supported",attrib);
        return NULL;
    }
    return NULL;
}

static void subcomponent_load_resources(Iterator *iter, void *arg) 
{
    Component *component;
    void *window = (void *)arg;
    uint8_t *addr;

    addr = (uint8_t *)iter->get_vpointer(iter);

    component = (Component *)buffer_to_addr(addr);
    if (component->load_resources)
        component->load_resources(component, window);
}

static int __load_resources(Component *component,void *window)
{
    Container *container = (Container *)component;

    dbg_str(DBG_SUC,"%s load resources",component->name);
    container->for_each_component(container, subcomponent_load_resources, window);

    return 0;
}

static int __unload_resources(Component *component,void *window)
{
    //...........
}

static void subcomponent_draw(Iterator *iter, void *arg) 
{
    Component *component;
    uint8_t *addr;
    Graph *g = (Graph *)arg;

    addr = (uint8_t *)iter->get_vpointer(iter);
    component = (Component *)buffer_to_addr(addr);

    if (component->draw) component->draw(component, g);
}

static int __draw(Component *component, void *graph)
{
    Container *container = (Container *)component;
    Graph *g = (Graph *)graph;
    dbg_str(DBG_SUC,"%s draw", ((Obj *)component)->name);

    container->for_each_component(container, subcomponent_draw, g);
}

static void __text_key_input(Component *component,char c, void *graph)
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    dbg_str(DBG_DETAIL,"EVENT: text input %c", c);
    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }

    if (cur->text_key_input) cur->text_key_input(cur,c, g);
}

static void __up_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->down_key_down) cur->down_key_down(cur, g); 

}

static void __down_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->down_key_down) cur->down_key_down(cur, g); 
}

static void __left_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->left_key_down) cur->left_key_down(cur, g); 
}

static void __right_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->right_key_down) cur->right_key_down(cur, g); 
}

static void __pageup_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->pageup_key_down) cur->pageup_key_down(cur, g); 
}

static void __pagedown_key_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->pagedown_key_down) cur->pagedown_key_down(cur, g); 
}

static void __one_line_up(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->one_line_up) cur->one_line_up(cur, g); 
}

static void __one_line_down(Component *component, void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->one_line_down) cur->one_line_down(cur, g); 
}

static void __backspace_key_input(Component *component,void *graph) 
{
    Container *container = (Container *)component;
    Graph *g             = (Graph *)graph;
    Component *cur;

    cur = container->search_component(container,"text_area");
    if (cur == NULL) {
        dbg_str(DBG_WARNNING,"not found component :%s","text_field");
        return;
    }
    if (cur->backspace_key_input) cur->backspace_key_input(cur, g); 
}

static void subcomponent_mouse_button_down(Iterator *iter, void *arg) 
{
    Graph *g       = (Graph *)arg;
    __Event *event = (__Event *)arg;
    Component *component;
    uint8_t *addr;

    addr = (uint8_t *)iter->get_vpointer(iter);
    component = (Component *)buffer_to_addr(addr);

    if (component->mouse_button_down) component->mouse_button_down(component, event, event->window);
}

static void __mouse_button_down(Component *component,void *event, void *window) 
{
    Container *container = (Container *)component;
    Window *w            = (Window *)window;
    Graph *g             = w->graph;
    __Event *e           = (__Event *)event;
    Component *cur;

    dbg_str(DBG_DETAIL,
            "%s process mouse_button_down event: Mouse button %d pressed at %d,"
            "%d with click count %d in window %d",
            component->name, e->button, e->x, e->y, e->clicks, e->windowid); 

    container->for_each_component(container, subcomponent_mouse_button_down, event);
}

static void subcomponent_mouse_over(Iterator *iter, void *arg) 
{
    Graph *g       = (Graph *)arg;
    __Event *event = (__Event *)arg;
    Component *component;
    uint8_t *addr;

    addr = (uint8_t *)iter->get_vpointer(iter);
    component = (Component *)buffer_to_addr(addr);

    if (component->mouse_over) component->mouse_over(component, event, event->window);
}


static void __mouse_over(Component *component,void *event, void *window) 
{
    Container *container = (Container *)component;
    Window *w            = (Window *)window;
    Graph *g             = w->graph;
    __Event *e           = (__Event *)event;
    Component *cur;

    /*
     *dbg_str(DBG_DETAIL, "EVENT: Mouse: moved to %d,%d (%d,%d) in window %d",
     *        e->x, e->y, e->xrel, e->yrel, e->windowid);
     */
    container->for_each_component(container, subcomponent_mouse_over, event);

}

static void __mouse_wheel(Component *component,void *event, void *window) 
{
    __Event *e           = (__Event *)event;

    dbg_str(DBG_DETAIL, "EVENT: Mouse: wheel scrolled %d in x and %d in y (direction: %d) in window %d", 
            e->x, e->y, e->direction, e->windowid);
}

static void __window_moved(Component *component,void *event, void *window) 
{
    dbg_str(DBG_DETAIL,"window moved");
}

static void __window_resized(Component *component,void *event, void *window) 
{
    __Event *e           = (__Event *)event;

    dbg_str(DBG_DETAIL,"EVENT: Window %d resized to %dx%d",
            e->data1, e->data2, e->windowid);
}

static class_info_entry_t component_class_info[] = {
    [0 ] = {ENTRY_TYPE_OBJ,"Container","container",NULL,sizeof(void *)},
    [1 ] = {ENTRY_TYPE_FUNC_POINTER,"","set",__set,sizeof(void *)},
    [2 ] = {ENTRY_TYPE_FUNC_POINTER,"","get",__get,sizeof(void *)},
    [3 ] = {ENTRY_TYPE_FUNC_POINTER,"","construct",__construct,sizeof(void *)},
    [4 ] = {ENTRY_TYPE_FUNC_POINTER,"","deconstruct",__deconstrcut,sizeof(void *)},
    [5 ] = {ENTRY_TYPE_FUNC_POINTER,"","move",__move,sizeof(void *)},
    [6 ] = {ENTRY_TYPE_VFUNC_POINTER,"","draw",__draw,sizeof(void *)},
    [7 ] = {ENTRY_TYPE_VFUNC_POINTER,"","load_resources",__load_resources,sizeof(void *)},
    [8 ] = {ENTRY_TYPE_VFUNC_POINTER,"","text_key_input",__text_key_input,sizeof(void *)},
    [9 ] = {ENTRY_TYPE_VFUNC_POINTER,"","backspace_key_input",__backspace_key_input,sizeof(void *)},
    [10] = {ENTRY_TYPE_VFUNC_POINTER,"","up_key_down",__up_key_down,sizeof(void *)},
    [11] = {ENTRY_TYPE_VFUNC_POINTER,"","down_key_down",__down_key_down,sizeof(void *)},
    [12] = {ENTRY_TYPE_VFUNC_POINTER,"","left_key_down",__left_key_down,sizeof(void *)},
    [13] = {ENTRY_TYPE_VFUNC_POINTER,"","right_key_down",__right_key_down,sizeof(void *)},
    [14] = {ENTRY_TYPE_VFUNC_POINTER,"","pageup_key_down",__pageup_key_down,sizeof(void *)},
    [15] = {ENTRY_TYPE_VFUNC_POINTER,"","pagedown_key_down",__pagedown_key_down,sizeof(void *)},
    [16] = {ENTRY_TYPE_VFUNC_POINTER,"","one_line_up",__one_line_up,sizeof(void *)},
    [17] = {ENTRY_TYPE_VFUNC_POINTER,"","one_line_down",__one_line_down,sizeof(void *)},
    [18] = {ENTRY_TYPE_VFUNC_POINTER,"","mouse_button_down",__mouse_button_down,sizeof(void *)},
    [19] = {ENTRY_TYPE_VFUNC_POINTER,"","mouse_over",__mouse_over,sizeof(void *)},
    [20] = {ENTRY_TYPE_VFUNC_POINTER,"","mouse_wheel",__mouse_wheel,sizeof(void *)},
    [21] = {ENTRY_TYPE_VFUNC_POINTER,"","window_moved",__window_moved,sizeof(void *)},
    [22] = {ENTRY_TYPE_VFUNC_POINTER,"","window_resized",__window_resized,sizeof(void *)},
    [23] = {ENTRY_TYPE_STRING,"char","name",NULL,0},
    [24] = {ENTRY_TYPE_END},

};
REGISTER_CLASS("Component",component_class_info);

void test_ui_component()
{
    Subject *subject;
    allocator_t *allocator = allocator_get_default_alloc();
    char *set_str;
    cjson_t *root, *e, *s;
    char buf[2048];

    root = cjson_create_object();{
        cjson_add_item_to_object(root, "Component", e = cjson_create_object());{
            cjson_add_item_to_object(e, "Subject", s = cjson_create_object());{
                cjson_add_number_to_object(s, "x", 1);
                cjson_add_number_to_object(s, "y", 25);
                cjson_add_number_to_object(s, "width", 5);
                cjson_add_number_to_object(s, "height", 125);
            }
            cjson_add_string_to_object(e, "name", "alan");
        }
    }

    set_str = cjson_print(root);

    /*
     *subject = OBJECT_ALLOC(allocator,Component);
     *object_set(subject, "Component", set_str);
     *dbg_str(DBG_DETAIL,"x=%d y=%d width=%d height=%d",subject->x,subject->y,subject->width,subject->height);
     */

    subject = OBJECT_NEW(allocator, Component,set_str);

    /*
     *dbg_str(DBG_DETAIL,"x=%d y=%d width=%d height=%d",subject->x,subject->y,subject->width,subject->height);
     *dbg_str(DBG_DETAIL,"component nane=%s",((Component *)subject)->name);
     *subject->move(subject);
     */

    object_dump(subject, "Component", buf, 2048);
    dbg_str(DBG_DETAIL,"Component dump: %s",buf);

    free(set_str);

}


