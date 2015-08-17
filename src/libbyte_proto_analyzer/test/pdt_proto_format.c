/*
 * =====================================================================================
 *
 *       Filename:  pdt_proto_format.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/10/2015 19:35:28 
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan 
 *        Company:  vigor
 *
 * =====================================================================================
 */
#include "libdbg/debug.h"
#include "libbyte_proto_analyzer/protocol_format_set.h"
#include "libbyte_proto_analyzer/pdt_proto_format.h"

int pfs_set_proto_format_3008(protocol_format_set_t *pfs_p)
{
	struct list_head *hl_head;
	int llist_num;

	hl_head = pfs_create_head_list(pfs_p->allocator);
	pfs_init_head_list(hl_head);

	PFS_SET_PROTO_INFO( PROTO_NUM,    "0",  "2", NULL,         hl_head);
	PFS_SET_PROTO_INFO( LAB,          "2",  "2", NULL,         hl_head);
	PFS_SET_PROTO_INFO( SLOT_INFO,    "4",  "2", NULL,         hl_head);
	PFS_SET_PROTO_INFO( SYNC_INFO,    "6",  "2", NULL,         hl_head);
	PFS_SET_PROTO_INFO( TACT_INFO,    "8",  "1",  NULL,         hl_head);
	PFS_SET_PROTO_INFO( SLC_INFO, 	  "9",  "1",  NULL,         hl_head);
	PFS_SET_PROTO_INFO( SLC_DATA,     "10", "0",  SLC_INFO, 	hl_head);
	PFS_SET_PROTO_INFO( SYNC_INDEX,   "10", "1",  NULL,         hl_head);
	PFS_SET_PROTO_INFO( ST_INFO,      "11", "1",  NULL,         hl_head);
	PFS_SET_PROTO_INFO( DC_INFO,      "12", "1",  NULL,         hl_head);
	PFS_SET_PROTO_INFO( DC_DATA,      "13", "0",  DC_INFO,      hl_head);

	llist_num =0x3008 - pfs_p->proto_base_addr;
	pfs_add_proto_link_list(llist_num,
			hl_head,//struct list_head *new_proto_llist,
			pfs_p);//struct list_head **list_head)
}