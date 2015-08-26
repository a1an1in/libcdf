/*
 * =====================================================================================
 *
 *       Filename:  allocator_cds_mempool_list.c 
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/15/2015 06:45:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan 
 *        Company:  
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
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include "libdbg/debug.h"
#include "liballoc/inc_files.h"

void *mempool_release_list(cds_mempool_t *mempool);
/*-----------------------------------------------------------------------------
 *  mempool list operation
 *-----------------------------------------------------------------------------*/
void mempool_init_head_list(struct list_head **hl_head,uint8_t lock_type)
{
	cds_mempool_head_list_t *head_list;

	head_list = (cds_mempool_head_list_t *)malloc(sizeof(cds_mempool_head_list_t));
	if(head_list == NULL){
		dbg_str(DBG_ALLOC_ERROR,"malloc mempool list_head_list");
		return;
	}
	/*
	 *pthread_rwlock_init(&head_list->head_lock,NULL);
	 */
	sync_lock_init(&head_list->head_lock,lock_type);
	head_list->count = 0;
	INIT_LIST_HEAD(&head_list->list_head);
	*hl_head = &head_list->list_head;
}
void mempool_attach_list(struct list_head *new_head,struct list_head *hl_head)
{
	cds_mempool_head_list_t *head_list;

	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);

	sync_lock(&head_list->head_lock,NULL);
	/*
	 *pthread_rwlock_wrlock(&head_list->head_lock);
	 */
	list_add(new_head,hl_head);
	head_list->count++;
	sync_unlock(&head_list->head_lock);
	/*
	 *pthread_rwlock_unlock(&head_list->head_lock);
	 */

	/*
	 *dbg_str(DBG_ALLOC_DETAIL,"add head:%p hl_head:%p",new_head,hl_head);
	 *dbg_str(DBG_ALLOC_DETAIL,"new heads next:%p, prev:%p",new_head->next,new_head->prev);
	 *dbg_str(DBG_ALLOC_DETAIL,"add mempool list,list count =%d",head_list->count);
	 */
}
/* not del list,just detach it from one link list */
void mempool_detach_list(struct list_head *del_head,struct list_head *hl_head)
{
	cds_mempool_head_list_t *head_list;
	cds_mempool_t *mempool_list;

	dbg_str(DBG_ALLOC_DETAIL,"mempool_detach_list");
	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);
	/*
	 *pthread_rwlock_wrlock(&head_list->head_lock);
	 */
	sync_lock(&head_list->head_lock,NULL);
	/*
	 *dbg_str(DBG_ALLOC_DETAIL,"mempool del head:%p, hl_head:%p",del_head,hl_head);
	 *dbg_str(DBG_ALLOC_DETAIL,"mempool del heads next:%p, prev:%p",del_head->next,del_head->prev);
	 */
	list_del(del_head);
	head_list->count--;
	mempool_list = container_of(del_head,cds_mempool_t,list_head);
	sync_unlock(&head_list->head_lock);
	/*
	 *pthread_rwlock_unlock(&head_list->head_lock);
	 */

	dbg_str(DBG_ALLOC_DETAIL,"del_mempool list,list count =%d",head_list->count);
}
void mempool_destroy_list(struct list_head *del_head,struct list_head *hl_head)
{
	cds_mempool_head_list_t *head_list;
	cds_mempool_t *mempool_list;

	dbg_str(DBG_ALLOC_DETAIL,"mempool_detach_list");
	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);
	/*
	 *pthread_rwlock_wrlock(&head_list->head_lock);
	 */
	sync_lock(&head_list->head_lock,NULL);
	/*
	 *dbg_str(DBG_ALLOC_DETAIL,"mempool del head:%p, hl_head:%p",del_head,hl_head);
	 *dbg_str(DBG_ALLOC_DETAIL,"mempool del heads next:%p, prev:%p",del_head->next,del_head->prev);
	 */
	list_del(del_head);
	head_list->count--;
	mempool_list = container_of(del_head,cds_mempool_t,list_head);
	mempool_release_list(mempool_list); 
	/*
	 *pthread_rwlock_unlock(&head_list->head_lock);
	 */
	sync_unlock(&head_list->head_lock);

	dbg_str(DBG_ALLOC_DETAIL,"del_mempool list,list count =%d",head_list->count);
}
void mempool_print_head_list(struct list_head *hl_head)
{
	cds_mempool_head_list_t *head_list;
	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);
	dbg_str(DBG_ALLOC_DETAIL,"mempool_print_head_list:mempool count:%d",head_list->count);
}
void mempool_print_list(cds_mempool_t *mempool_list)
{
	dbg_str(DBG_ALLOC_DETAIL,"mempool info,start addr:%p,depth:%d,min_depth:%d,size=%d,list_head_addr:%p",
			mempool_list->start,
			mempool_list->depth,
			mempool_list->min_depth,
			mempool_list->size,
			&mempool_list->list_head);
}
void mempool_print_list_for_each(struct list_head *hl_head)
{
	cds_mempool_head_list_t *head_list;
	cds_mempool_t *mempool_list;
	struct list_head *pos,*n;

	mempool_print_head_list(hl_head);

	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);

	/*
	 *pthread_rwlock_rdlock(&head_list->head_lock);
	 */
	sync_lock(&head_list->head_lock,NULL);
	list_for_each_safe(pos, n, hl_head) {
		mempool_list = container_of(pos,cds_mempool_t,list_head);
		mempool_print_list(mempool_list);
	}
	sync_unlock(&head_list->head_lock);
	/*
	 *pthread_rwlock_unlock(&head_list->head_lock);
	 */
}

cds_mempool_t *mempool_create_list(allocator_t *alloc)
{
	cds_alloc_t *alloc_p = &alloc->priv.cds_alloc; 
	cds_mempool_t *mempool;
	void *p;

	p = malloc(sizeof(cds_mempool_t));
	if(p == NULL){
		dbg_str(DBG_ALLOC_ERROR,"malloc cds_mempool_t");
		return p;
	}else{
		mempool = (cds_mempool_t *)p;
	}
	p = malloc(alloc_p->mempool_capacity);
	if(p == NULL){
		dbg_str(DBG_ALLOC_ERROR,"malloc mem pool err");
		free(mempool);
		return p;
	}
	mempool->size = alloc_p->mempool_capacity;
	mempool->start = p;
	mempool->depth = alloc_p->mempool_capacity;
	mempool->min_depth = alloc_p->data_min_size + sizeof(cds_slab_t);

	return mempool;
}
void *mempool_release_list(cds_mempool_t *mempool)
{
	dbg_str(DBG_ALLOC_DETAIL,"mempool_release_list");
	free(mempool->start);
	free(mempool);
}
//not release lock
void mempool_destroy_lists(struct list_head *hl_head)
{
	struct list_head *pos,*n;

	dbg_str(DBG_ALLOC_DETAIL,"mempool_destroy_lists");
	list_for_each_safe(pos, n, hl_head) {
		dbg_str(DBG_ALLOC_DETAIL,"run at here");
		mempool_destroy_list(pos,hl_head);
	}
}
cds_mempool_t *mempool_find_appropriate_pool(allocator_t *alloc,uint32_t size)
{
	cds_alloc_t *alloc_p = &alloc->priv.cds_alloc; 
	struct list_head *hl_head = alloc_p->pool;
	struct list_head *empty_pool_hl_head = alloc_p->empty_pool;
	cds_mempool_head_list_t *head_list;
	cds_mempool_t *mempool_list;
	struct list_head *pos,*n;

	head_list = container_of(hl_head,cds_mempool_head_list_t,list_head);

	/*
	 *pthread_rwlock_rdlock(&head_list->head_lock);
	 */
	sync_lock(&head_list->head_lock,NULL);
	list_for_each_safe(pos, n, hl_head) {
		mempool_list = container_of(pos,cds_mempool_t,list_head);
		if(mempool_list->depth >= size + sizeof(cds_slab_t)){
			dbg_str(DBG_ALLOC_DETAIL,"find an appropriate_pool");
			/*
			 *pthread_rwlock_unlock(&head_list->head_lock);
			 */
			sync_unlock(&head_list->head_lock);
			return mempool_list;
		}else if(mempool_list->depth < mempool_list->min_depth){
			dbg_str(DBG_ALLOC_WARNNING,"this mempool is empty");
			/*
			 *pthread_rwlock_unlock(&head_list->head_lock);
			 */
			sync_unlock(&head_list->head_lock);
			mempool_detach_list(pos,hl_head);
			mempool_attach_list(pos,empty_pool_hl_head);
			/*
			 *pthread_rwlock_rdlock(&head_list->head_lock);
			 */
			sync_lock(&head_list->head_lock,NULL);
		}
	}
	/*
	 *pthread_rwlock_unlock(&head_list->head_lock);
	 */
	sync_unlock(&head_list->head_lock);
	return NULL;
}
uint32_t get_real_alloc_mem_size(allocator_t *alloc,uint32_t size)
{
	int data_min_size = alloc->priv.cds_alloc.data_min_size;
	return (slab_get_slab_index(alloc,size) + 1 ) * data_min_size;
}
cds_slab_t *mempool_alloc_slab_list(allocator_t *alloc,uint32_t size)
{

	cds_mempool_t *mempool_list;
	cds_alloc_t *alloc_p = &alloc->priv.cds_alloc;
	cds_slab_t *slab_list;

	dbg_str(DBG_ALLOC_DETAIL,"mempool_alloc_slab_list");

	if(!(mempool_list = mempool_find_appropriate_pool(alloc,size))){
		dbg_str(DBG_ALLOC_WARNNING,"not find appropriate_pool,create a new pool");
		if(mempool_list = mempool_create_list(alloc)){
			mempool_attach_list(&mempool_list->list_head,alloc_p->pool);
		}else{
			dbg_str(DBG_ALLOC_ERROR,"can't create a new mempool_list");
			return NULL;
		}
	}

	dbg_str(DBG_ALLOC_DETAIL,"dmempool_alloc_slab_list  next:%p, prev:%p",
			mempool_list->list_head.next,mempool_list->list_head.prev);

	slab_list = mempool_list->start + mempool_list->size - mempool_list->depth;
	slab_list->size      = get_real_alloc_mem_size(alloc,size);
	slab_list->data_size = size;
	slab_list->mem_addr  = &slab_list->data[0];
	slab_list->stat_flag = 1;
	slab_list->slab_size = slab_list->size + sizeof(cds_slab_t);

	assert(slab_list->size >= slab_list->data_size);

	/*
	 *dbg_str(DBG_ALLOC_DETAIL,"slab_list=%p,slab_list->mem_addr=%p", slab_list ,slab_list->mem_addr);
	 *dbg_str(DBG_ALLOC_DETAIL,"sizeof(cds_slab_t)=%d,size=%d",sizeof(cds_slab_t),size);
	 *dbg_str(DBG_ALLOC_DETAIL,"dmempool_alloc_mem:%p,slab list_head:%p",slab_list->mem_addr,&slab_list->list_head);
	 *dbg_str(DBG_ALLOC_DETAIL,"dmempool_alloc_slab_list  next:%p, prev:%p",
	 *        mempool_list->list_head.next,mempool_list->list_head.prev);
	 */
	
	mempool_list->depth -= slab_list->slab_size;

	return slab_list;
}
