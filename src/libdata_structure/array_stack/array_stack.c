#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libdbg/debug.h>
#include <libdata_structure/array_stack.h>

array_stack_t *array_stack_alloc(allocator_t *allocator)
{
     array_stack_t *as;

     if( (as = (array_stack_t *)
               allocator_mem_alloc(allocator,
                                   sizeof(array_stack_t))) == NULL )
     {
         dbg_str(DBG_DETAIL,"allocator_mem_alloc");
         return NULL;
     }

     memset(as, 0 , sizeof(array_stack_t));

     as->allocator = allocator;

     return as;
}

int array_stack_init(array_stack_t *as)
{
    int ret = 0;

    if(as->size == 0) {
        as->size = 100;
    }
    
    if(as->step == 0) {
        as->step = 8;
    }

    as->top = (uint8_t *)allocator_mem_alloc(as->allocator,as->size);
    if(as->top == NULL) {
        dbg_str(DBG_WARNNING,"allocator_mem_alloc");
        return -1;
    }
    as->cur = as->top;

    return ret;
}

int array_stack_push(array_stack_t *as, void *data)
{
    int ret = 0;
    
    if(as->cur - as->top + as->step < as->size) {
        memset(as->cur, 0, as->step);
        memcpy(as->cur, data, as->step);
        dbg_buf(DBG_DETAIL,"array_stack_push:",data, as->step);
        as->cur += as->step;
    } else {
        ret = -1;
        dbg_str(DBG_WARNNING,"array stack is full");
    }

    return ret;
}

int array_stack_pop(array_stack_t *as,void *out)
{
    int ret = 0;

    if(as->cur > as->top) {
        memset(out, 0, as->step);
        memcpy(out, as->cur - as->step, as->step);
        dbg_buf(DBG_DETAIL,"array_stack_pop:",as->cur - as->step, as->step);
        as->cur -= as->step;
    } else {
        dbg_str(DBG_WARNNING,"array stack is null");
        ret = -1;
    }

    return ret;
}

int array_stack_destroy(array_stack_t *as)
{
    int ret = 0;

    allocator_mem_free(as->allocator,as->top);
    allocator_mem_free(as->allocator,as);

    return ret;
}

int test_array_stack()
{
    int ret = 0;
    array_stack_t *as;
    allocator_t *allocator = allocator_get_default_alloc();
    int p;
    int a;

    as = array_stack_alloc(allocator);
    array_stack_init(as);

    dbg_str(DBG_DETAIL,"as addr:%x",as);
    array_stack_push(as, &as);
    a = 5;
    array_stack_push(as, &a);
    a = 6;
    array_stack_push(as, &a);
    a = 7;
    array_stack_push(as, &a);


    array_stack_pop(as, &p);
    dbg_str(DBG_DETAIL,"pop data:%x",p);
    array_stack_pop(as, &p);
    dbg_str(DBG_DETAIL,"pop data:%x",p);
    array_stack_pop(as, &p);
    dbg_str(DBG_DETAIL,"pop data:%x",p);
    array_stack_pop(as, &p);
    dbg_str(DBG_DETAIL,"pop data:%x",p);
    array_stack_pop(as, &p);
    dbg_str(DBG_DETAIL,"pop data:%x",p);

    return ret;
}
