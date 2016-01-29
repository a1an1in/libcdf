/*
 * =====================================================================================
 *
 *       Filename:  vector.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2015 02:47:46 PM
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
#include <libdata_structure/vector.h>
#include <libdbg/debug.h>


int vector_copy_backward(vector_t *vector,vector_pos_t *to,vector_pos_t *from,uint32_t count)
{
	uint32_t from_pos, to_pos;
	void *vector_head = vector->vector_head;
	uint32_t step     = vector->step;
	uint32_t num_per;
	
	from_pos   = from->vector_pos + count;
	to_pos     = to->vector_pos + count;

	num_per = to_pos - from_pos;
	for(;count > 0;){
		to_pos   -= num_per;
		from_pos -= num_per;
		count    -= num_per;
		num_per   = count - num_per > 0 ? num_per :count;
		dbg_str(DBG_DETAIL,"to_pos=%d,from_pos=%d",to_pos,from_pos);
		memcpy(vector_head + to_pos * step, vector_head + from_pos * step, num_per * step);
	}
}
int vector_copy_forward(vector_t *vector,vector_pos_t *to,vector_pos_t *from,uint32_t count)
{
	uint32_t from_pos = from->vector_pos;
	uint32_t to_pos   = to->vector_pos;
	void *vector_head = vector->vector_head;
	uint32_t step     = vector->step;
	uint32_t num_per;
	
	num_per = from_pos - to_pos;
	for(;count > 0;){
		num_per = count - num_per > 0 ? num_per :count;
		dbg_str(DBG_DETAIL,"to_pos=%d,from_pos=%d,num_per=%d",to_pos,from_pos,num_per);
		memcpy(vector_head + to_pos * step,vector_head + from_pos * step,num_per * step);
		to_pos += num_per;
		from_pos += num_per;
		count -= num_per;
	}
}
int vector_copy(vector_t *vector,vector_pos_t *to,vector_pos_t *from,uint32_t count)
{
	uint32_t from_pos = from->vector_pos;
	uint32_t to_pos   = to->vector_pos;
	
	dbg_str(DBG_WARNNING,"count=%d",count);
	if(from_pos > to_pos){//forward
		vector_copy_forward(vector,to,from,count);
	}else{//backward
		vector_copy_backward(vector,to,from,count);
	}
}
vector_t *vector_create(allocator_t *allocator,uint8_t lock_type)
{
	vector_t *ret = NULL;
	ret = (vector_t *)allocator_mem_alloc(allocator,sizeof(vector_t));
	if(ret == NULL){
		dbg_str(DBG_ERROR,"allock err");
	}
	ret->allocator = allocator;
	ret->lock_type = lock_type;
	return ret;
}
int vector_init(vector_t *vector,uint32_t data_size,uint32_t capacity)
{

	dbg_str(DBG_DETAIL,"vector init");
	vector->step = data_size;
	vector->data_size = data_size;
	vector->capacity = capacity;
	vector->vector_head = allocator_mem_alloc(
			vector->allocator, capacity * (vector->step));
	if(vector->vector_head == NULL){
		dbg_str(DBG_ERROR,"vector_init");
	}
	dbg_str(DBG_DETAIL,"vector_head:%p,size=%d",
			vector->vector_head,
			capacity * (vector->step));
	vector_pos_init(&vector->begin,0,vector);
	vector_pos_init(&vector->end,0,vector);

	sync_lock_init(&vector->vector_lock,vector->lock_type);

	return 0;
}
int vector_push_front(vector_t *vector,void *data)
{
	dbg_str(DBG_WARNNING,"not support vector_push_front");
	return 0;
}
int vector_push_back(vector_t *vector,void *data)
{
	uint32_t data_size    = vector->data_size;
	void *vector_head     = vector->vector_head;
	uint32_t step         = vector->step;
	uint32_t capacity     = vector->capacity;
	uint32_t push_pos     = vector->end.vector_pos;

	sync_lock(&vector->vector_lock,NULL);
	if(push_pos < capacity){
		memcpy(vector_head + (push_pos++)*step,data,data_size);
		vector_pos_init(&vector->end,push_pos,vector);
	}else{
		dbg_str(DBG_WARNNING,"realloc mem");
		vector->vector_head = allocator_mem_alloc(
				vector->allocator, 2*capacity * (vector->step));
		if(vector->vector_head == NULL){
			dbg_str(DBG_ERROR,"vector_push_back,realloc mem");
		}
		vector->capacity = 2*capacity;
		memcpy(vector->vector_head,vector_head,capacity*step);
		memcpy(vector->vector_head + (push_pos++)*step,data,data_size);
		vector_pos_init(&vector->end,push_pos,vector);
		allocator_mem_free(vector->allocator,vector_head);
	}
	sync_unlock(&vector->vector_lock);
	dbg_str(DBG_DETAIL,"vector_push_back,push_pos=%d,capacity=%d",push_pos,vector->capacity);

	return 0;
}

int vector_pop_front(vector_t *vector)
{
	dbg_str(DBG_WARNNING,"not support vector_pop_front");
	return 0;
}
int vector_pop_back(vector_t *vector)
{
	uint32_t pop_pos  = vector->end.vector_pos;
	
	dbg_str(DBG_DETAIL,"pop back");

	sync_lock(&vector->vector_lock,NULL);
	if(!vector_pos_equal(&vector->begin,&vector->end)){
		vector_pos_init(&vector->end,--pop_pos,vector);
	} else{
		dbg_str(DBG_WARNNING,"vector is NULL");
	}
	sync_unlock(&vector->vector_lock);

	return 0;
}
int vector_insert(vector_t *vector, vector_pos_t *it, void *data)
{

	uint32_t insert_pos = it->vector_pos;
	uint32_t end_pos    = vector->end.vector_pos;
	uint32_t count      = end_pos - insert_pos;
	void *vector_head   = vector->vector_head;
	uint32_t step       = vector->step;
	vector_pos_t to;
	
	vector_pos_init(&to,insert_pos + 1,vector);
	dbg_str(DBG_DETAIL,"insert_pos=%d,to_pos=%d",insert_pos,to.vector_pos);

	sync_lock(&vector->vector_lock,NULL);
	vector_copy(vector,&to,it,count);
	memcpy(vector_head + insert_pos * step,data,step);
	vector_pos_init(&vector->end,end_pos + 1,vector);
	sync_unlock(&vector->vector_lock);

	return 0;
}
int vector_delete(vector_t *vector, vector_pos_t *it)
{
	uint32_t delete_pos  = it->vector_pos;
	uint32_t end_pos = vector->end.vector_pos;
	uint32_t count = end_pos - delete_pos;
	vector_pos_t from;

	vector_pos_init(&from,delete_pos + 1,vector);

	sync_lock(&vector->vector_lock,0);
	if(vector_pos_equal(it,&vector->end)){
		dbg_str(DBG_WARNNING,"can't del end pos");
	}else if(vector_pos_equal(it,&vector->begin)&&vector_pos_equal(&from,&vector->end)){
		dbg_str(DBG_WARNNING,"vector is NULL");
	}else{
		vector_copy(vector,it,&from,count);
		vector_pos_init(&vector->end,end_pos - 1,vector);
	}
	sync_unlock(&vector->vector_lock);

	return 0;
}
int vector_set(vector_t *vector,int index,void *data)
{
	uint32_t set_pos  = index;
	uint32_t end_pos  = vector->end.vector_pos;
	void *vector_head = vector->vector_head;
	uint32_t step     = vector->step;
	int ret  = 0;
	
	dbg_str(DBG_DETAIL,"set_pos=%d",set_pos);

	sync_lock(&vector->vector_lock,NULL);
	memcpy(vector_head + set_pos * step,data,step);
	if(set_pos > end_pos){
		vector_pos_init(&vector->end,set_pos,vector);
	}
	sync_unlock(&vector->vector_lock);

	return ret;
}
void * vector_get(vector_t *vector,int index)
{
	uint32_t get_pos     = index;
	uint8_t *vector_head = vector->vector_head;
	uint32_t step        = vector->step;
	void * ret           = NULL;
	
	dbg_str(DBG_DETAIL,"get_pos=%d",get_pos);

	sync_lock(&vector->vector_lock,NULL);
	ret = (vector_head + get_pos * step);
	sync_unlock(&vector->vector_lock);

	return ret;
}
int vector_destroy(vector_t *vt)
{
	dbg_str(DBG_DETAIL,"vector_destroy");

	sync_lock_destroy(&vt->vector_lock);

	allocator_mem_free(vt->allocator,vt->vector_head);
	allocator_mem_free(vt->allocator,vt);
}


