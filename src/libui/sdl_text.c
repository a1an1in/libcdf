/**
 * @file text_sdl.c
 * @synopsis 
 * @author a1an1in@sina.com
 * @version 
 * @date 2016-12-04
 */
#include <stdio.h>
#include <libdbg/debug.h>
#include <libui/sdl_text.h>
#include <libui/sdl_window.h>

static int __construct(Text *text,char *init_str)
{
	dbg_str(OBJ_DETAIL,"text construct, text addr:%p",text);

	return 0;
}

static int __deconstrcut(Text *text)
{
	Sdl_Text *i = (Sdl_Text *)text;
	dbg_str(OBJ_DETAIL,"text deconstruct,text addr:%p",text);

	if(i->surface != NULL){
		SDL_FreeSurface(i->surface);
	}

	return 0;
}

static int __set(Text *text, char *attrib, void *value)
{
	Sdl_Text *i = (Sdl_Text *)text;

	if(strcmp(attrib, "set") == 0) {
		i->set = value;
    } else if(strcmp(attrib, "get") == 0) {
		i->get = value;
	} else if(strcmp(attrib, "construct") == 0) {
		i->construct = value;
	} else if(strcmp(attrib, "deconstruct") == 0) {
		i->deconstruct = value;
	} else if(strcmp(attrib, "load_text") == 0) {
		i->load_text = value;
	} else {
		dbg_str(OBJ_WARNNING,"text set,  \"%s\" setting is not support",attrib);
	}

	return 0;
}

static void * __get(Text *text, char *attrib)
{
    if(strcmp(attrib, "x") == 0){ 
    } else {
        dbg_str(OBJ_WARNNING,"text get, \"%s\" getting attrib is not supported",attrib);
        return NULL;
    }
    return NULL;
}

static int __load_text(Text *text,void *font)
{
	Sdl_Text *t = (Sdl_Text *)text;
	Sdl_Font *f = (Sdl_Font *)font;
	SDL_Color textColor = { 0, 0, 0, 0xFF };

	dbg_str(DBG_SUC,"Sdl_Text load text");
	t->surface = TTF_RenderText_Solid(f->ttf_font, text->content->value, textColor ); 
}

static class_info_entry_t text_class_info[] = {
	[0 ] = {ENTRY_TYPE_OBJ,"Text","text",NULL,sizeof(void *)},
	[1 ] = {ENTRY_TYPE_FUNC_POINTER,"","set",__set,sizeof(void *)},
	[2 ] = {ENTRY_TYPE_FUNC_POINTER,"","get",__get,sizeof(void *)},
	[3 ] = {ENTRY_TYPE_FUNC_POINTER,"","construct",__construct,sizeof(void *)},
	[4 ] = {ENTRY_TYPE_FUNC_POINTER,"","deconstruct",__deconstrcut,sizeof(void *)},
	[5 ] = {ENTRY_TYPE_FUNC_POINTER,"","load_text",__load_text,sizeof(void *)},
	[6 ] = {ENTRY_TYPE_END},

};
REGISTER_CLASS("Sdl_Text",text_class_info);

void test_obj_sdl_text()
{
    Window *window;
	Graph *g;
	Text *text;
	Font *font;
	allocator_t *allocator = allocator_get_default_alloc();
    char *set_str;
    char buf[2048];

    set_str = gen_window_setting_str();

    window  = OBJECT_NEW(allocator, Sdl_Window,set_str);
	g       = window->graph;

    object_dump(window, "Sdl_Window", buf, 2048);
    dbg_str(DBG_DETAIL,"Window dump: %s",buf);

	/*
     *font = OBJECT_NEW(allocator, Sdl_Font,"");
	 *font->load_font(font);
	 */

    text = OBJECT_NEW(allocator, Sdl_Text,"");
	text->content->assign(text->content,"hello world");
	g->render_load_text(g,text,window->font, 0,0,0,0xff);
	g->render_write_text(g,0,0,text);

	text->content->assign(text->content,"a");
	g->render_load_text(g,text,window->font, 0,0,0,0xff);
	g->render_write_text(g,0,33,text);

	text->content->assign(text->content,"b");
	g->render_load_text(g,text,window->font, 0,0,0,0xff);
	g->render_write_text(g,0,66,text);

	text->content->assign(text->content,"i");
	g->render_load_text(g,text,window->font, 0,0,0,0xff);
	g->render_write_text(g,0,99,text);

	g->render_present(g);

	pause();

    object_destroy(window);

    free(set_str);

}

