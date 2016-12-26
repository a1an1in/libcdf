#ifndef __ITERATOR_HMAP_H__
#define __ITERATOR_HMAP_H__

#include <stdio.h>
#include <libdbg/debug.h>
#include <libobject/iterator.h>
#include <libdata_structure/link_list.h>

typedef struct llist_iterator_s LList_Iterator;

struct llist_iterator_s{
	Iterator iter;

	int (*construct)(Iterator *iter,char *init_str);
	int (*deconstruct)(Iterator *iter);
	int (*set)(Iterator *iter, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);

    /*virtual funcs*/
    Iterator *(*next)(Iterator *it);
    Iterator *(*prev)(Iterator *it);
    int (*equal)(Iterator *it1,Iterator *it2);
    void *(*get_vpointer)(Iterator *it);
    void *(*get_kpointer)(Iterator *it);

	/*virtual methods reimplement*/
#define MAX_NAME_LEN 50
    char name[MAX_NAME_LEN];
#undef MAX_NAME_LEN
    list_pos_t list_pos;

};

#endif