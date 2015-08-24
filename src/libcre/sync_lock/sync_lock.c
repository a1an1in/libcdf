/*
 * =====================================================================================
 *
 *       Filename:  sync_lock.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/24/2015 02:08:23 PM
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
#include <stdlib.h>
#include "libcre/sync_lock/sync_lock.h"

sync_lock_module_t sync_lock_modules[SYNC_LOCK_TYPE_MAX_NUM];

inline int sync_lock_init(struct sync_lock_s *slock,uint32_t sync_lock_type)
{
	slock->lock_ops = &sync_lock_modules[sync_lock_type].sl_ops;

	return slock->lock_ops->sync_lock_init(slock);
}
inline int sync_lock(struct sync_lock_s *slock,uint32_t flag)
{
	return slock->lock_ops->sync_lock(slock,flag);
}
inline int sync_trylock(struct sync_lock_s *slock,uint32_t flag)
{
	return slock->lock_ops->sync_trylock(slock,flag);
}
inline int sync_unlock(struct sync_lock_s *slock)
{
	return slock->lock_ops->sync_unlock(slock);
}
inline int sync_lock_destroy(struct sync_lock_s *slock)
{
	return slock->lock_ops->sync_lock_destroy(slock);
}

