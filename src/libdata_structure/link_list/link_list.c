/*
 * =====================================================================================
 *
 *       Filename:  list_t.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
/*  
 * Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 *  
 *  
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
#include <stdio.h>
#include <string.h>
#include "libdbg/debug.h"
#include "libdata_structure/link_list.h"


llist_t *llist_create(allocator_t *allocator,uint8_t lock_type)
{
    llist_t *ret = NULL;
    dbg_str(LINKLIST_IMPORTANT,"llist_create");
    ret = (llist_t *)allocator_mem_alloc(allocator,sizeof(llist_t));
    if(ret == NULL){
        dbg_str(LINKLIST_ERROR,"allock err");
    }
    ret->allocator = allocator;
    ret->lock_type = lock_type;
    return ret;
}
int llist_init(llist_t *llist,uint32_t data_size)
{
    struct list_head *p;
    dbg_str(LINKLIST_DETAIL,"llist init");

    llist->data_size = data_size;
    llist->list_count = 0;
    //only assigned head,if without head,llist is hard to distinguish head or end;
    p = (struct list_head *)allocator_mem_alloc(llist->allocator, sizeof(struct list_head));
    if(p == NULL){
        dbg_str(LINKLIST_ERROR,"allock err");
    }
    INIT_LIST_HEAD(p);
    llist_pos_init(&llist->begin,p,llist);
    llist_pos_init(&llist->head,p,llist);

    sync_lock_init(&llist->list_lock,llist->lock_type);

    return 0;
}
int llist_insert(llist_t *llist, list_pos_t *pos, void *data)
{
    list_t *p = NULL;
    uint32_t data_size = llist->data_size; 

    p = (list_t *)allocator_mem_alloc(llist->allocator,sizeof(list_t) + data_size);
    memcpy(p->data,data,data_size);

    sync_lock(&llist->list_lock,NULL);
    list_add(&p->list_head, pos->list_head_p);
    if(llist_pos_equal(pos,&llist->head)){
        llist_pos_init(&llist->begin,&p->list_head,llist);
    }
    llist->list_count++;

    dbg_str(LINKLIST_DETAIL,"insert llist,listcount=%d,addr=%p",llist->list_count,&p->list_head);

    sync_unlock(&llist->list_lock);

    return 0;
}
int llist_delete(llist_t *llist, list_pos_t *pos)
{
    list_t *p;

    if(llist_pos_equal(&llist->begin,&llist->head)){
        dbg_str(LINKLIST_WARNNING,"llist is null,llist_delete");
        return -1;
    }

    p = container_of(pos->list_head_p,list_t,list_head);

    sync_lock(&llist->list_lock,NULL);
    if(llist_pos_equal(pos,&llist->begin)){
        llist_pos_init(&llist->begin,pos->list_head_p->next,llist);
    }
    list_del(pos->list_head_p);
    llist->list_count--;
    dbg_str(LINKLIST_DETAIL,"delete llist,listcount=%d",llist->list_count);

    sync_unlock(&llist->list_lock);

    allocator_mem_free(llist->allocator,p);
    return 0;
}
list_t *__llist_detach(llist_t *llist, list_pos_t *pos)
{
    list_t *p;

    if(llist_pos_equal(&llist->begin,&llist->head)){
        dbg_str(LINKLIST_DETAIL,"llist is null,llist_detach");
        return NULL;
    }

    p = container_of(pos->list_head_p,list_t,list_head);

    list_del(pos->list_head_p);
    if(llist_pos_equal(pos,&llist->begin)){
        llist_pos_init(&llist->begin,llist->head.list_head_p->next,llist);//if this list is first,updata begin
    }
    llist->list_count--;
    dbg_str(LINKLIST_DETAIL,"detach llist,listcount=%d",llist->list_count);

    return p;
}
int llist_push_back(llist_t *llist,void *data)
{
    list_t *p = NULL;
    uint32_t data_size = llist->data_size; 

    p = (list_t *)allocator_mem_alloc(llist->allocator,sizeof(list_t) + data_size);
    memcpy(p->data,data,data_size);

    sync_lock(&llist->list_lock,NULL);
    list_add_tail(&p->list_head, llist->head.list_head_p);
    if(llist_pos_equal(&llist->head,&llist->begin)){
        llist_pos_init(&llist->begin,llist->head.list_head_p->next,llist);//if this list is first,updata begin
    }
    llist->list_count++;
    /*
     *dbg_str(LINKLIST_DETAIL,"llist_push_back,listcount=%d,addr=%p",llist->list_count,&p->list_head);
     */

    sync_unlock(&llist->list_lock);

    return 0;
}

int llist_pop_back(llist_t *llist)
{
    list_t *p;
    struct list_head *head = llist->head.list_head_p;

    if(llist_pos_equal(&llist->begin,&llist->head)){
        dbg_str(LINKLIST_WARNNING,"llist is null,llist_pop_back");
        return -1;
    }

    p = container_of(head->prev,list_t,list_head);

    sync_lock(&llist->list_lock,NULL);
    list_del(head->prev);
    llist->list_count--;
    if(llist->list_count == 0){
        llist_pos_init(&llist->begin,llist->head.list_head_p,llist);
    }
    dbg_str(LINKLIST_DETAIL,"llist_pop_back,listcount=%d",llist->list_count);

    sync_unlock(&llist->list_lock);

    allocator_mem_free(llist->allocator,p);
    return 0;
}

list_t *llist_detach_back(llist_t *llist)
{
    list_t *p;
    struct list_head *head = llist->head.list_head_p;

    if(llist_pos_equal(&llist->begin,&llist->head)){
        dbg_str(LINKLIST_WARNNING,"llist is null,llist_detach_back");
        return NULL;
    }

    p = container_of(head->prev,list_t,list_head);

    sync_lock(&llist->list_lock,NULL);
    list_del(head->prev);
    llist->list_count--;
    if(llist->list_count == 0){
        llist_pos_init(&llist->begin,llist->head.list_head_p,llist);
    }
    dbg_str(LINKLIST_DETAIL,"llist_detach_back,listcount=%d",llist->list_count);

    sync_unlock(&llist->list_lock);

    return p;
}
int llist_destroy(llist_t *llist)
{
    list_pos_t pos,next;

    dbg_str(LINKLIST_IMPORTANT,"llist_destroy");

    for(    llist_begin(llist, &pos), llist_pos_next(&pos,&next);
            !llist_pos_equal(&pos,&llist->head);
            pos = next, llist_pos_next(&pos,&next))
    {
        llist_delete(llist,&pos);
    }
    if(llist_pos_equal(&llist->head,&llist->begin)){
        dbg_str(LINKLIST_DETAIL,"llist_destroy,llist is NULL,free llist head");
        allocator_mem_free(llist->allocator,llist->head.list_head_p);
        sync_lock_destroy(&llist->list_lock);
    }

    return 0;
}

