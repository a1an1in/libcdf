/*
 * =====================================================================================
 *
 *       Filename:  debug.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/24/2015 03:57:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libdbg/debug.h"
#include "libdbg/debug_string.h"

debugger_t *debugger_gp;
debugger_module_t debugger_modules[MAX_DEBUGGER_MODULES_NUM];

void debugger_set_level_info(debugger_t *debugger,
		uint32_t level,uint8_t color_value,const char *level_str)
{
	debugger->debug_level_info[level].color = color_value;
	memcpy(debugger->debug_level_info[level].level_str,level_str,strlen(level_str));
	/*
	 *printf("%s %x\n",console_print_color[debug_devel].level,console_print_color[debug_devel].color);
	 */
}
void debugger_set_level_infos(debugger_t *debugger)
{
	debugger_set_level_info(debugger, DBG_PANIC,    (DBG_PANIC_COLOR),    "DBG_PANIC");
	debugger_set_level_info(debugger, DBG_FATAL,    (DBG_FATAL_COLOR),    "DBG_FATAL");
	debugger_set_level_info(debugger, DBG_ERROR,    (DBG_ERROR_COLOR),    "DBG_ERROR");
	debugger_set_level_info(debugger, DBG_WARNNING, (DBG_WARNNING_COLOR), "DBG_WARNNING");
	debugger_set_level_info(debugger, DBG_SUC,      (DBG_SUC_COLOR),      "DBG_SUC");
	debugger_set_level_info(debugger, DBG_CORRECT,  (DBG_CORRECT_COLOR),  "DBG_CORRECT");
	debugger_set_level_info(debugger, DBG_VIP,      (DBG_VIP_COLOR),      "DBG_VIP");
	debugger_set_level_info(debugger, DBG_FLOW,     (DBG_FLOW_COLOR),     "DBG_FLOW");
	debugger_set_level_info(debugger, DBG_IMPORTANT,(DBG_IMPORTANT_COLOR),"DBG_IMPORTANT");
	debugger_set_level_info(debugger, DBG_DETAIL, 	(DBG_DETAIL_COLOR),   "DBG_DETAIL");
}
uint8_t debugger_get_level_color(debugger_t *debugger,uint32_t level)
{
	return debugger->debug_level_info[level].color;
}
uint8_t* debugger_get_level_str(debugger_t *debugger,uint32_t level)
{
	return debugger->debug_level_info[level].level_str;
}


void debugger_set_business(debugger_t *debugger,
		uint32_t business_num,uint8_t on_off,uint8_t debug_level)
{
	debugger->debug_business[business_num].business_switch = on_off;
	debugger->debug_business[business_num].business_debug_level = debug_level;
}
/*
 *enum debug_business_enum{
 *    DBG_BUSINESS_NORMAL = 0,
 *    DBG_BUSINESS_ALLOC,
 *    DBG_BUSINESS_CONTAINER,
 *    MAX_DEBUG_BUSINESS_NUM
 *};
 */
void debugger_set_businesses(debugger_t *debugger)
{
	dictionary *d = debugger->d;;
	int bussiness_num,i;
#define MAX_STRING_LEN 50
	char switch_str[MAX_STRING_LEN];
	char level_str[MAX_STRING_LEN];
	int sw,lv;

	bussiness_num = iniparser_getint(d, (char *)"businesses:business_num",0);
	if(bussiness_num == 0){
		iniparser_setstr(d, (char *)"businesses", NULL); 
		iniparser_setstr(d, (char *)"businesses:business_num", "3"); 
		for(i = 0; i < MAX_DEBUG_BUSINESS_NUM; i++){
			snprintf(switch_str,MAX_STRING_LEN,"businesses:business%d_switch",i);
			snprintf(level_str,MAX_STRING_LEN,"businesses:business%d_level",i);
			iniparser_setstr(d, switch_str,"1");
			iniparser_setstr(d, level_str,"9");
			debugger_set_business(debugger,i,1,9);
		}
		FILE *f = fopen(debugger->ini_file_name, "w");
		iniparser_dump_ini(d, f);
		fclose(f);
	}else{
		for(i = 0; i < bussiness_num; i++){
			snprintf(switch_str,MAX_STRING_LEN,"businesses:business%d_switch",i);
			snprintf(level_str,MAX_STRING_LEN,"businesses:business%d_level",i);
			printf("debugger_set_businesses,bussiness_num=%d\n",bussiness_num);
			sw = iniparser_getint(d, switch_str,1);
			lv = iniparser_getint(d, level_str,9);
			debugger_set_business(debugger,i,sw,lv);
		}
	}
#undef MAX_BUSINESSES_SETTING_LEN
}

int debugger_is_business_switch_on(debugger_t *debugger,uint32_t business_num)
{
	return debugger->debug_business[business_num].business_switch == 1;
}
int debugger_get_business_level(debugger_t *debugger,uint32_t business_num)
{
	return debugger->debug_business[business_num].business_debug_level;
}

void debugger_init(debugger_t *debugger)
{
	if(debugger->dbg_ops->init)
		debugger->dbg_ops->init(debugger);
}
void debugger_destroy(debugger_t *debugger)
{
	if(debugger->dbg_ops->destroy)
		debugger->dbg_ops->destroy(debugger);
}
/*
 *int debugger_dbg_str(debugger_t *debugger,uint32_t dbg_switch,const char *fmt,...) 
 *{
 *    int ret = 0;
 *    va_list ap;
 *    uint32_t business_num = dbg_switch >> 8;
 *    uint8_t level = dbg_switch & 0xff;
 *    if(!debugger_is_business_switch_on(debugger,business_num)){
 *        return -1;
 *    }
 *    if(debugger_get_business_level(debugger,business_num) < level){
 *        return -1;
 *    }
 *    va_start(ap,fmt);
 *    ret = debugger->dbg_ops->dbg_string_vl(debugger,level,fmt,ap);
 *    va_end(ap);
 *    return ret;
 *}
 */
int debugger_dbg_str(debugger_t *debugger,uint32_t dbg_switch,const char *fmt,...) 
{
#define MAX_FMT_STR_LEN 1024*4
	int ret = 0;
	va_list ap;
	uint32_t business_num = dbg_switch >> 8;
	uint8_t level = dbg_switch & 0xff;
	char fmt_str[MAX_FMT_STR_LEN];
	char *level_str;

	if(!debugger_is_business_switch_on(debugger,business_num)){
		return -1;
	}
	if(debugger_get_business_level(debugger,business_num) < level){
		return -1;
	}
	va_start(ap,fmt);
	vsnprintf(fmt_str,MAX_FMT_STR_LEN,fmt,ap);
	va_end(ap);
	level_str = (char *)debugger_get_level_str(debugger,level);
	ret = debugger->dbg_ops->dbg_string(debugger,level,"[%s]--%s",level_str,fmt_str);

	return ret;
#undef MAX_FMT_STR_LEN 
}
int debugger_dbg_buf(debugger_t *debugger,
		uint32_t dbg_switch,const char* const_str,
		uint8_t *buf,uint32_t buf_len, const char *fmt,...) 
{
#define MAX_BUFFER_STR_LEN 1024*4
#define MAX_FMT_STR_LEN 1024
	int ret = 0;
	va_list ap;
	uint32_t business_num = dbg_switch >> 8;
	uint8_t level = dbg_switch & 0xff;
	char buffer_str[MAX_BUFFER_STR_LEN];
	char fmt_str[MAX_FMT_STR_LEN];
	char *level_str;

	if(!debugger_is_business_switch_on(debugger,business_num)){
		return -1;
	}
	if(debugger_get_business_level(debugger,business_num) < level){
		return -1;
	}
	debug_string_buf_to_str(buf,buf_len,buffer_str,MAX_BUFFER_STR_LEN);
	va_start(ap,fmt);
	vsnprintf(fmt_str,MAX_FMT_STR_LEN,fmt,ap);
	va_end(ap);
	level_str = (char*)debugger_get_level_str(debugger,level);
	ret = debugger->dbg_ops->dbg_string(debugger,level,"[%s]--[%s%s]--%s",level_str,const_str,buffer_str,fmt_str);

	return ret;
#undef MAX_BUFFER_STR_LEN
#undef MAX_FMT_STR_LEN 
}

debugger_t *debugger_creator(char *ini_file_name)
{
	debugger_t *debugger;
	int8_t type;
	dictionary *d;
	FILE *f;

	debugger =(debugger_t *)malloc(sizeof(debugger_t));

	debugger->d = d = iniparser_new(ini_file_name);
	memcpy(debugger->ini_file_name,ini_file_name,strlen(ini_file_name));

	type = iniparser_getint(d, (char *)"debugger:type", 0);
	if(access(debugger->ini_file_name,F_OK)){
		printf("ini file not exsit\n");
		f = fopen(debugger->ini_file_name, "w");
		iniparser_setstr(d, (char *)"debugger", NULL); 
		/*
		 *itoa(type,type_str,10);
		 */
		iniparser_setstr(d, (char *)"debugger:type","0");
		iniparser_dump_ini(d, f);
		fclose(f);
	}
	printf("debugger type =%d\n",type);
	debugger->debugger_type = type;
	debugger->dbg_ops = &debugger_modules[type].dbg_ops;


	debugger_set_level_infos(debugger);
	debugger_set_businesses(debugger);

	return debugger;

}
