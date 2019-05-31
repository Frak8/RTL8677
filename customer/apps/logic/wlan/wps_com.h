  
/******************************************************************************
  Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  �ļ�����: wps_com.h
  �ļ�����: WPSģ��Ĺ���ͷ�ļ�����Ҫ����ģ����Ժ�ȹ�������
  �����б�: //�г���Ҫ����
  �޶���¼:
         1. ����: Hansel He < heyiming@twsz.com>
            ����: 2008-8-11
            ����: �����ļ�
 ******************************************************************************/


#ifndef __WPS_COM_H__
#define __WPS_COM_H__
#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/****************************************************************************** 
 *                                 MACRO                                      * 
 ******************************************************************************/

#define ACTION_CALL                     "CALL"
#define ACTION_RETURN                   "RETURN"

#if 0
#define WPS_PRINT(format, args...)		printf("[%s:%d:%s()] ==> "format, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define WPS_PRINT(format, args...)		TBS_TRACE(MID_WLAN, format, ##args)
#endif

#ifdef WLAN_WPS_DEBUG
#define WPS_TRACE(format, args...)		WPS_PRINT(format, ##args)
#define WPS_ASSERT(cond)                 do{ \
                                            if(!(cond)){ \
                                               WPS_PRINT("ASSERT failed!\n"); \
                                               exit(-1);} \
                                          }while(0)
#define WPS_FUNC_TRACE(action, arg_fmt, args...)		WPS_TRACE("%s: %s("arg_fmt") ...... \n", action,  \
                                                            __FUNCTION__, ##args)                                          
#define WPS_FUNC_ENTER_TRACE(arg_fmt, args...)	WPS_FUNC_TRACE(ACTION_CALL,arg_fmt, ##args)
#define WPS_FUNC_EXIT_TRACE()			WPS_FUNC_TRACE(ACTION_RETURN,"")
#else
#define WPS_TRACE(format, args...)
#define WPS_ASSERT(cond) 
#define WPS_FUNC_TRACE(action, arg_fmt, args...)
#define WPS_FUNC_ENTER_TRACE(arg_fmt, args...)
#define WPS_FUNC_EXIT_TRACE()
#endif




#define WPS_ERROR(format, args...)      printf("[WPS:ERROR] "format, ##args)
#define WPS_INFO(format, args...)       printf("[WPS:INFO] "format, ##args)

#define SYSTEM_CMD_MAX_LEN              256
#define SYSTEM_CMD(cmd)                 tbsSystem((cmd), 0)

#define WPS_MATCH_CMD(buf, cmd)            (safe_strncmp((buf), (cmd), strlen(cmd)) == 0)

#define RUN_SYS_CMD(format, args...)    do{ \
                                            char cmd[SYSTEM_CMD_MAX_LEN]; \
                                            snprintf(cmd, SYSTEM_CMD_MAX_LEN, format, ##args); \
                                            SYSTEM_CMD(cmd); \
                                        }while(0)
/****************************************************************************** 
 *                                 ENUM                                       * 
 ******************************************************************************/
typedef enum {
    NODE_NOT_SAVE,
    NODE_NEED_SAVE        
}EM_NODE_NEEDSAVE;

typedef enum {
    NODE_UNSTANDARD,
    NODE_STANDARD        
}EM_NODE_STANDARD;

typedef enum {
    NODE_UNWRITEABLE,
    NODE_WRITEABLE        
}EM_NODE_WRITEABLE;
/****************************************************************************** 
 *                                STRUCT                                      * 
 ******************************************************************************/










/****************************************************************************** 
 *                               FUNCTION                                     * 
 ******************************************************************************/










/****************************************************************************** 
 *                                 END                                        * 
 ******************************************************************************/
#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /* __WPS_COM_H__ */



