#ifndef __FONT_SDL_H__
#define __FONT_SDL_H__

#include <libobject/obj.h>
#include <libobject/string.h>
#include <libui/font.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct sdl_font_s Sdl_Font;

struct sdl_font_s{
	Font font;

	/*normal methods*/
	int (*construct)(Font *font,char *init_str);
	int (*deconstruct)(Font *font);
	int (*set)(Font *font, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);

	/*virtual methods*/
	int (*load_font)(Font *font);

	/*attribs*/
	TTF_Font *ttf_font;
};

#endif