#ifndef __BORDERLAYOUT_H__
#define __BORDERLAYOUT_H__

#include <stdio.h>
#include <libdbg/debug.h>
#include <libui/component.h>
#include <libui/text.h>
#include <libui/color.h>
#include <libui/cursor.h>

typedef struct border_layout_s Border_Layout;

enum border_layout_pos_e{
    BORDER_LAYOUT_NORTH = 0,
    BORDER_LAYOUT_WEST,
    BORDER_LAYOUT_CENTER,
    BORDER_LAYOUT_EAST,
    BORDER_LAYOUT_SOUTH
};

struct border_layout_s{
	Component component;

	int (*construct)(Border_Layout *border_layout,char *init_str);
	int (*deconstruct)(Border_Layout *border_layout);
	int (*set)(Border_Layout *border_layout, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);

	/*virtual methods reimplement*/
    int (*add_component)(Container *obj, void *pos, void *component);
	int (*load_resources)(Component *component, void *window);
	int (*draw)(Component *component, void *graph);

#define MAX_NAME_LEN 50
    char name[MAX_NAME_LEN];
#undef MAX_NAME_LEN
    void *window;
    uint32_t *col_width, *row_height; 
    uint32_t hgap, vgap;
    uint32_t row_max, col_max, cur_row, cur_col;
    uint32_t default_unit_width, default_unit_height;
    uint32_t layout_width, layout_height;

    Component **grid_components;
};

#endif