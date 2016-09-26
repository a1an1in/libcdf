/*
 * =====================================================================================
 *
 *       Filename:  link_list.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2015 09:30:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

#include "libdata_structure/link_list_struct.h"
#include <libdbg/debug.h>

llist_t *llist_create(allocator_t *allocator,uint8_t lock_type);
int llist_init(llist_t *llist,uint32_t data_size);
int llist_insert(llist_t *llist, list_pos_t *pos, void *data);
int llist_delete(llist_t *llist, list_pos_t *pos);
int llist_push_back(llist_t *llist,void *data);
int llist_pop_back(llist_t *llist);
int llist_destroy(llist_t *llist);
list_t *__llist_detach(llist_t *llist, list_pos_t *pos);


static inline int 
llist_pos_init(list_pos_t *pos,struct list_head *lh,llist_t *llist)
{

	pos->llist = llist;
	pos->list_head_p = lh;

	return 0;
}
static inline int 
llist_begin(llist_t *llist,list_pos_t *begin)
{
	list_pos_t *pos = &llist->begin;
	return llist_pos_init(begin,pos->list_head_p,pos->llist);
}
static inline int 
llist_end(llist_t *llist,list_pos_t *end)
{
	list_pos_t *pos = &llist->head;
	return llist_pos_init(end,pos->list_head_p,pos->llist);
}
static inline int 
llist_pos_next(list_pos_t *pos,list_pos_t *next)
{
	return llist_pos_init(next,pos->list_head_p->next,pos->llist);
}
static inline int 
llist_pos_prev(list_pos_t *pos,list_pos_t *prev)
{
	return llist_pos_init(prev,pos->list_head_p->prev,pos->llist);
}
static inline int llist_pos_equal(list_pos_t *pos1,list_pos_t *pos2)
{
	return pos1->list_head_p == pos2->list_head_p;
}
static inline int llist_push_front(llist_t *llist,void *data)
{
	return llist_insert(llist,&llist->head,data);
}
static inline int llist_pop_front(llist_t *llist)
{
	return llist_delete(llist, &llist->begin);
}
static inline list_t *llist_detach_front(llist_t *llist)
{
	list_t *l = NULL;

	sync_lock(&llist->list_lock,NULL);
	l = __llist_detach(llist, &llist->begin);
	sync_unlock(&llist->list_lock);

	return l;
}
static inline list_t *llist_detach(llist_t *llist, list_pos_t *pos)
{
	list_t *l = NULL;

	sync_lock(&llist->list_lock,NULL);
	l = __llist_detach(llist, pos);
	sync_unlock(&llist->list_lock);

	return l;
}
static inline int llist_get_count(llist_t *llist)
{
	return llist->list_count;
}
static inline void *llist_pos_get_pointer(list_pos_t *pos)
{
	return (list_t *)(container_of(pos->list_head_p,list_t,list_head))->data;
}
static inline void llist_for_each(llist_t *llist,void (*func)(list_t *list))
{
	list_t *ls;
	list_pos_t pos,next;

	for(	llist_begin(llist, &pos), llist_pos_next(&pos,&next);
			!llist_pos_equal(&pos,&llist->head);
			pos = next, llist_pos_next(&pos,&next))
	{
		ls = container_of(pos.list_head_p,list_t,list_head);
		func(ls);
	}
}
static inline void llist_for_each_with_arg(llist_t *llist,void (*func)(list_t *list,void *arg),void *arg)
{
	list_t *ls;
	list_pos_t pos,next;

	for(	llist_begin(llist, &pos), llist_pos_next(&pos,&next);
			!llist_pos_equal(&pos,&llist->head);
			pos = next, llist_pos_next(&pos,&next))
	{
		ls = container_of(pos.list_head_p,list_t,list_head);
		func(ls,arg);
	}
}
static inline void llist_for_each_with_arg2(llist_t *llist,void (*func)(list_t *list,void *arg1,void *arg2),void *arg1,void *arg2)
{
	list_t *ls;
	list_pos_t pos,next;

	for(	llist_begin(llist, &pos), llist_pos_next(&pos,&next);
			!llist_pos_equal(&pos,&llist->head);
			pos = next, llist_pos_next(&pos,&next))
	{
		ls = container_of(pos.list_head_p,list_t,list_head);
		func(ls,arg1,arg2);
	}
}

#endif
